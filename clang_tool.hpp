#include <clang/AST/ExprCXX.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Tooling/ArgumentsAdjusters.h>
#include <clang/Tooling/Tooling.h>
#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Expr.h>
#include <clang/Basic/Version.h>

#include <iostream>
#include <llvm/Support/Casting.h>

using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;

// Searched list in TopLevelStmtVisitor needs to be updated with these
// Save function names
const std::string save = "save";

// Same implementation
const std::string save_complex = "save_complex";
const std::string save_complex_ptr = "save_complex_ptr";

const std::string save_data = "save_data";
const std::string save_process = "save_process";
const std::string save_enum = "save_enum";

// === Utility ===
void assert_error(const bool& truthy, const char* message) {
    if (truthy) return;
    std::cerr << message << '\n';
    exit(-1);
}

// ---------------------------- TODO virtual class with a virtual save function for saving below's data. Multiple different classes with one central save function is required.

// === Visitor to analyze function body ===
class TopLevelStmtVisitor : public RecursiveASTVisitor<TopLevelStmtVisitor> {
public:
    explicit TopLevelStmtVisitor(ASTContext *Ctx)
        : Context(Ctx), Depth(0) {}

    bool TraverseStmt(Stmt *S) {
        if (!S) return true;
        bool isControl = isa<IfStmt>(S) || isa<ForStmt>(S) || isa<WhileStmt>(S)
                      || isa<DoStmt>(S) || isa<SwitchStmt>(S);

        if (isControl) Depth++; // Check depth of call
        bool result = RecursiveASTVisitor::TraverseStmt(S);
        if (isControl) Depth--;
        return result;
    }

    bool VisitCallExpr(CallExpr *call) {
        if (Depth == 0) {
            if (FunctionDecl* called = call->getDirectCallee()) // No function ptr support
            {
                std::string name = called->getNameAsString();

                const Expr *arg = call->getArg(0)->IgnoreUnlessSpelledInSource()->IgnoreParenLValueCasts();

                arg->dumpColor();

                std::string var_name;
                std::string var_type;
                if (auto *dr = dyn_cast<DeclRefExpr>(arg)) { // Locally created var
                    var_name = dr->getDecl()->getName();
                }
                else if (auto *ce = dyn_cast<CXXConstructExpr>(arg)) { // Constructed via conversion var // TODO make sure it is not const
                    const MemberExpr* temp = dyn_cast<MemberExpr>(ce->getArg(0)); // Kinda bad TODO as it only checks one conversion layer

                    assert_error(temp, "Constant / Combined saving is a waste of time and money. LValue must be just a Member Variable"); // TODO !!! FIX THIS TO PROPERLY INCLUDE CONVERSIONS

                    ValueDecl* member_temp = temp->getMemberDecl();
                    QualType member_type = ce->getType(); // Outer conversion type

                    var_type = member_type.getAsString();
                    var_name = member_temp->getName();
                }
                else if (auto *me = dyn_cast<MemberExpr>(arg)) // Direct Member Object var
                {
                    ValueDecl* member_temp = me->getMemberDecl();
                    QualType member_type = arg->getType();

                    var_type = member_type.getAsString();
                    var_name = member_temp->getName();
                }

                if (name == save)
                {
                    std::cout << "  Call to save (inherited): " << var_name << "\n";
                    // Get inherited call
                }

                else if (name == save_complex || name == save_complex_ptr)
                {
                    std::cout << "  Call to " << name << ": " << var_name << ": " << var_type << "\n";
                    // Get template type of called and save name
                }

                else if (name == save_data)
                {
                    std::cout << "  Call to save_data: " << var_name << ": " << var_type << "\n";
                    // Get the template type of called and save name
                }

                else if (name == save_enum)
                {
                    std::cout << "  Call to save_enum: " << var_name << "\n";
                    // Same as save_data, but also save the enum name
                }

                else if (name == save_process)
                {
                    std::cout << "  Call to save_process: " << var_name << "\n";
                    // Get the var name and type of process
                }
            }
        }

        else std::cout << "Only base level saving, no statements!\n";

        return true;
    }

private:
    ASTContext *Context;
    int Depth;
};

// === Match callback ===
class MethodFinderCallback : public MatchFinder::MatchCallback {
public:
    void run(const MatchFinder::MatchResult &Result) override {
        const auto *Method = Result.Nodes.getNodeAs<CXXMethodDecl>("saveMethod");
        assert_error(Method, "Bad Match Error");

        if (!Method->hasBody()) {
            std::cout << "Matched method has no body.\n";
            return;
        }

        std::cout << "Matched method: " << Method->getNameAsString() << "\n";

        // Run the visitor on this method's body
        TopLevelStmtVisitor Visitor(Result.Context);
        Visitor.TraverseStmt(Method->getBody());
    }
};

#define MACRO_STR(macro) STR(macro)
#define STR(a) #a

int main() {
    std::string errorMsg; // TODO While error, compile project
    auto Compilations = CompilationDatabase::loadFromDirectory("Myne/build/", errorMsg);
    if (!Compilations) {
        std::cout << "Error loading compilation database: " << errorMsg << "\n";
        return 1;
    }

    std::vector<std::string> files = {
        "Myne/.cpp/object.cpp"  // Update this path
    };

    ClangTool Tool(*Compilations, files);

    // Add clang includes for the CLANG TOOL!
    #ifdef __linux__
    std::string arg = "/usr/bin/../lib/clang/";
    arg += std::string(MACRO_STR(__clang_major__));
    arg += "/include";

    Tool.appendArgumentsAdjuster(getInsertArgumentAdjuster("-I"));
    Tool.appendArgumentsAdjuster(getInsertArgumentAdjuster(arg.c_str()));
    #endif

    #ifdef _WIN32
    #error "OS NOT SUPPORTED"
    CRASH!
    #endif

    #ifdef __APPLE__
    #error "OS NOT SUPPORTED"
    CRASH!
    #endif

    MethodFinderCallback FinderCallback;
    MatchFinder Finder;

    DeclarationMatcher MethodMatcher =
        cxxMethodDecl(hasName(save), isExpansionInMainFile()).bind("saveMethod");

    Finder.addMatcher(MethodMatcher, &FinderCallback);

    return Tool.run(newFrontendActionFactory(&Finder).get());
}