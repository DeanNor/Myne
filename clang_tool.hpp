#include "err.hpp"
#include <cassert>
#include <clang/AST/ExprCXX.h>
#include <clang/AST/PrettyPrinter.h>
#include <clang/AST/Type.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Tooling/ArgumentsAdjusters.h>
#include <clang/Tooling/Tooling.h>
#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Expr.h>
#include <clang/Basic/Version.h>

#include <filesystem>
#include <iostream>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/Support/Casting.h>
#include <string>

using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;

const static std::string load_id = "\1";

// Searched list in TopLevelStmtVisitor needs to be updated with these
// load function names
const static std::string load = "load";

// Same implementation
const static std::string load_complex = "load_complex";
const static std::string load_complex_ptr = "load_complex_ptr";

const static std::string load_data = "load_data";
const static std::string load_process = "load_process";
const static std::string load_enum = "load_enum";

// TODO make all the error windows here present to a buffer to be displayed

struct load_ast
{
public:
    virtual void use_editor() = 0;

    std::string name;

    load_ast(std::string variable_name) : name(variable_name)
    {

    }
};

struct vector_link
{
public:
    std::vector<load_ast*>* where;
    size_t where_inside;

    std::string variable_name;
    bool is_variable;

    vector_link(std::vector<load_ast*>* vector, size_t index) : where(vector), where_inside(index), is_variable(false)
    {

    }

    vector_link(std::vector<load_ast*>* vector, size_t index, std::string name) : where(vector), where_inside(index), variable_name(name), is_variable(true)
    {

    }
};

struct ast_expands : public load_ast
{
private:
    std::vector<load_ast*>* expandable_values;

public:
    ast_expands(std::string variable_name) : load_ast(variable_name)
    {
        expandable_values = new std::vector<load_ast*>;
    }

    void add_to_values(load_ast* val)
    {
        expandable_values->push_back(val);
    }

    vector_link* add_linked()
    {
        size_t size = expandable_values->size();
        expandable_values->push_back(0);

        return new vector_link(expandable_values, size);
    }

    vector_link* add_linked(std::string variable_name)
    {
        size_t size = expandable_values->size();
        expandable_values->push_back(0);

        return new vector_link(expandable_values, size, variable_name);
    }
};

struct ast_expands_complex : public ast_expands
{
public:
    ast_expands_complex(std::string variable_name) : ast_expands(variable_name)
    {
        
    }

    virtual void use_editor() override
    {

    }
};

struct ast_expands_process : public ast_expands
{
public:
    ast_expands_process(std::string variable_name) : ast_expands(variable_name)
    {

    }

    virtual void use_editor() override
    {
        // TODO as in we can not usefully store processes in variables like this. Maybe do also as an index of children?
    }
};

struct ast_value : public load_ast
{
public:
    QualType type;

    ast_value(std::string variable_name, QualType variable_type) : load_ast(variable_name), type(variable_type)
    {

    }

    virtual void use_editor() override
    {

    }
};

static std::unordered_map<std::string, vector_link*> to_be_linked;

inline std::string findAssignedVariable(const Stmt *stmt, ASTContext &Context)
{
    DynTypedNode node = DynTypedNode::create(*stmt);

    while (true) {
        auto parents = Context.getParents(node);
        if (parents.empty()) return "";

        node = parents[0];

        if (const auto *binOp = node.get<BinaryOperator>())
        {
            const Expr *lhs = binOp->getLHS()->IgnoreImpCasts();
            if (const auto *declRef = dyn_cast<DeclRefExpr>(lhs))
            {
                return declRef->getDecl()->getNameAsString();
            }
            if (const auto *memberExpr = dyn_cast<MemberExpr>(lhs))
            {
                if (const auto *field = dyn_cast<FieldDecl>(memberExpr->getMemberDecl()))
                {
                    return field->getNameAsString();
                }
            }
        }

        if (const auto *opCall = node.get<CXXOperatorCallExpr>())
        {
            if (opCall->getOperator() == OO_Equal && opCall->getNumArgs() >= 2)
            {
                const Expr *lhs = opCall->getArg(0)->IgnoreImpCasts();

                if (const auto *memberExpr = dyn_cast<MemberExpr>(lhs))
                {
                    if (const auto *field = dyn_cast<FieldDecl>(memberExpr->getMemberDecl()))
                    {
                        return field->getNameAsString();
                    }
                }

                if (const auto *declRef = dyn_cast<DeclRefExpr>(lhs))
                {
                    return declRef->getDecl()->getNameAsString();
                }
            }
        }

        if (const auto *varDecl = node.get<VarDecl>())
        {
            return varDecl->getNameAsString();
        }

        // Keep climbing if still in a Stmt
        if (const auto *stmtNode = node.get<Stmt>())
        {
            stmt = stmtNode;
        }
        
        else
        {
            return "";
        }
    }
}

class TopLevelStmtVisitor : public RecursiveASTVisitor<TopLevelStmtVisitor>
{
public:
    explicit TopLevelStmtVisitor(ASTContext *Ctx, ast_expands_process* root_ast) : Context(Ctx), policy(Context->getLangOpts()), ast(root_ast)
    {
        policy.SuppressTagKeyword = true;
    }

    bool TraverseStmt(Stmt *S)
    {
        if (!S) return true;
        bool isControl = isa<IfStmt>(S) || isa<ForStmt>(S) || isa<WhileStmt>(S)
                      || isa<DoStmt>(S) || isa<SwitchStmt>(S);

        if (isControl) Depth++; // Check depth of call
        bool result = RecursiveASTVisitor::TraverseStmt(S);
        if (isControl) Depth--;
        return result;
    }

    bool VisitCallExpr(CallExpr *call)
    {
        //call->dumpColor();

        if (Depth == 0)
        {
            CXXMemberCallExpr *call_function = dyn_cast<CXXMemberCallExpr>(call->IgnoreUnlessSpelledInSource());
            
            if (call_function)
            {
                std::string func_name = call_function->getMethodDecl()->getNameAsString();

                if (func_name == load)
                {
                    to_be_linked[call_function->getStmtClassName()] = ast->add_linked(); // TODO thread safety
                }

                else if (func_name == load_data)
                {
                    std::string var_name = findAssignedVariable(call, *Context);
                    ASSERT(!var_name.empty(), "No variable name found");

                    auto template_arg = call_function->getMethodDecl()->getTemplateSpecializationArgs();
                    ASSERT(template_arg->size() == 1, "Error with function type's templates");

                    auto var_type = template_arg->get(0).getAsType();

                    std::cout << "Var: " << var_name << ' ' << var_type.getAsString(policy) << '\n';

                    ast->add_to_values(new ast_value(var_name, var_type));
                }

                else if (func_name == load_complex || func_name == load_complex_ptr)
                {
                    std::string var_name = findAssignedVariable(call, *Context);
                    ASSERT(!var_name.empty(), "No variable name found");

                    auto template_arg = call_function->getMethodDecl()->getTemplateSpecializationArgs();
                    ASSERT(template_arg->size() == 1, "Error with function type's templates");

                    auto var_type = template_arg->get(0).getAsType();

                    std::cout << "Var: " << var_name << ' ' << var_type.getAsString(policy) << '\n';

                    to_be_linked[var_type.getAsString()] = ast->add_linked(var_name); // TODO thread safety
                }

                else if (func_name == load_process)
                {
                                        std::string var_name = findAssignedVariable(call, *Context);
                    ASSERT(!var_name.empty(), "No variable name found");

                    auto template_arg = call_function->getMethodDecl()->getTemplateSpecializationArgs();
                    ASSERT(template_arg->size() == 1, "Error with function type's templates");

                    auto var_type = template_arg->get(0).getAsType();

                    std::cout << "Var: " << var_name << ' ' << var_type.getAsString(policy) << '\n';

                    to_be_linked[var_type.getAsString()] = ast->add_linked(var_name); // TODO thread safety
                }

                else if (func_name == load_enum)
                {
                    std::string var_name = findAssignedVariable(call, *Context);
                    ASSERT(!var_name.empty(), "No variable name found");

                    auto template_arg = call_function->getMethodDecl()->getTemplateSpecializationArgs();
                    ASSERT(template_arg->size() == 1, "Error with function type's templates");

                    auto var_type = template_arg->get(0).getAsType();

                    std::cout << "Var: " << var_name << ' ' << var_type.getAsString(policy) << '\n';

                    ast->add_to_values(new ast_value(var_name, var_type));

                    // TODO find enum values and store them

                }
            }
        }

        return true;
    }

private:
    ASTContext *Context;
    PrintingPolicy policy;

    int Depth = 0;

    ast_expands_process* ast;
};

// === Match callback ===
class MethodFinderCallback : public MatchFinder::MatchCallback {
private:
    ast_expands_process* root_ast;

public:
    MethodFinderCallback(ast_expands_process* ast) : root_ast(ast)
    {

    }

    void run(const MatchFinder::MatchResult &Result) override {
        const auto *Method = Result.Nodes.getNodeAs<CXXMethodDecl>(load_id);
        ASSERT(Method, "Bad Match Error");

        if (!Method->hasBody()) {
            std::cout << "Matched method has no body.\n";
            return;
        }

        // Run the visitor on this method's body
        TopLevelStmtVisitor Visitor(Result.Context, root_ast);
        Visitor.TraverseStmt(Method->getBody());
    }
};

#define MACRO_STR(macro) STR(macro)
#define STR(a) #a

static std::filesystem::path build_dir = "./build";

inline load_ast* search_load(std::filesystem::path path) {
    std::string errorMsg; // TODO While error, compile project
    auto Compilations = CompilationDatabase::loadFromDirectory(build_dir.string(), errorMsg);
    if (!Compilations) {
        std::cout << "Error loading compilation database: " << errorMsg << "\n";
        return nullptr;
    }

    ClangTool Tool(*Compilations, {path}); // TODO static version of this so it does not need to be reloaded on runtime

    // Add clang includes for the CLANG TOOL!
    #ifdef __linux__
    std::string arg = "/usr/bin/../lib/clang/";
    arg += std::string(MACRO_STR(__clang_major__));
    arg += "/include";

    Tool.appendArgumentsAdjuster(getInsertArgumentAdjuster("-I"));
    Tool.appendArgumentsAdjuster(getInsertArgumentAdjuster(arg.c_str()));
    #endif

    #ifdef _WIN32
    std::string arg = "%VCINSTALLDIR%\\VC\\Tools\\MSVC\\"; // TODO Genuinely believe this does not work
    arg += std::string(MACRO_STR(__clang_major__));
    arg += "\\include";

    Tool.appendArgumentsAdjuster(getInsertArgumentAdjuster("-I"));
    Tool.appendArgumentsAdjuster(getInsertArgumentAdjuster(arg.c_str()));
    #endif

    #ifdef __APPLE__
    std::string arg = "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include"; // TODO Genuinely believe this does not work

    Tool.appendArgumentsAdjuster(getInsertArgumentAdjuster("-I"));
    Tool.appendArgumentsAdjuster(getInsertArgumentAdjuster(arg.c_str()));
    #endif

    ast_expands_process* ast = new ast_expands_process("Current Object");

    MethodFinderCallback FinderCallback(ast);
    MatchFinder Finder;

    DeclarationMatcher MethodMatcher =
        cxxMethodDecl(hasName(load), isExpansionInMainFile()).bind(load_id);

    Finder.addMatcher(MethodMatcher, &FinderCallback);

    bool success = Tool.run(newFrontendActionFactory(&Finder).get());
    if (success) return ast;
    return nullptr;
}