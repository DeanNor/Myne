
#include "pos.hpp"

pos::pos(const double& new_x, const double& new_y)
{
    x = new_x;
    y = new_y;
}

pos::pos()
{
    x = 0;
    y = 0;
}

pos::pos(const pos& past_pos)
{
    x = past_pos.x;
    y = past_pos.y;
}

pos::operator b2Vec2()
{
    return b2Vec2{(float)x,(float)y};
}

pos::pos(const b2Vec2& convert)
{
    x = convert.x;
    y = convert.y;
}

pos::operator SDL_FPoint()
{
    return SDL_FPoint{(float)x,(float)y};
}

pos::pos(const SDL_FPoint& convert)
{
    x = convert.x;
    y = convert.y;
}

SDL_FRect pos::Make_SDL_FRect(const pos& center, const pos& offset)
{
    return SDL_FRect{(float)(center.x - offset.x), (float)(center.y - offset.y), (float)(offset.x * 2.0f), (float)(offset.y * 2.0f)};
}

double pos::sum()
{
    double sum = std::abs(x) + std::abs(y);
    return sum;
}

rad pos::direction()
{
    rad dir = pos(0,0).angle_to(*this);
    return dir;
}

pos pos::ratio()
{
    pos x_to_y = {0,0};

    double scale = sum();

    x_to_y.x = x / scale;
    x_to_y.y = y / scale;

    return x_to_y;
}

double pos::len()
{
    double length = std::sqrt(std::pow(x,2) + std::pow(y,2));
    return length;
}

pos pos::rotated(rad angle)
{
    if (angle != rad(0.0))
    {
        double cosine = cos(angle);
        double sine = sin(angle);

        pos temp_pos = {};

        temp_pos.x = (x * cosine) - (y * sine);

        temp_pos.y = (x * sine) + (y * cosine);

        return temp_pos;
    }

    else
    {
        return *this;
    }
}

rad pos::angle_to(pos target)
{
    rad new_angle = atan2(target.y - y,target.x - x);
    return new_angle;
}

pos pos::limited(double limit)
{
    double len = std::abs(x) + std::abs(y);
    pos temp_pos = {};

    if (len > limit)
    {
        double ratio_over = (len) / limit;

        if (ratio_over > 0)
        {
            temp_pos.x = x / ratio_over;
            temp_pos.y = y / ratio_over;
        }
    }

    return temp_pos;
}

void pos::stop()
{
    x = 0;
    y = 0;
}

pos pos::scaled(pos start, pos end)
{
    pos temp_pos = *this;

    temp_pos *= end / start;
    
    return temp_pos;
}

bool pos::within(pos a, pos b)
{
    if(x < std::min(a.x, b.x) || x > std::max(a.x, b.x))
    {
        return false;
    }

    if (y < std::min(a.y,b.y) || y > std::max(a.y,b.y))
    {
        return false;
    }

    return true;
}

pos pos::floor()
{
    pos temp_pos = *this;

    temp_pos.x = std::floor(temp_pos.x);
    temp_pos.y = std::floor(temp_pos.y);

    return temp_pos;
}

pos pos::ceil()
{
    pos temp_pos = *this;

    temp_pos.x = std::ceil(temp_pos.x);
    temp_pos.y = std::ceil(temp_pos.y);

    return temp_pos;
}

pos pos::round()
{
    pos temp_pos = *this;

    temp_pos.x = std::round(temp_pos.x);
    temp_pos.y = std::round(temp_pos.y);

    return temp_pos;
}

pos& pos::operator= (const pos& convert)
{
    x = convert.x;
    y = convert.y;
    return *this;
}

pos pos::operator+ (const pos amount)
{
    pos temp_pos = *this;
    temp_pos.x += amount.x;
    temp_pos.y += amount.y;

    return temp_pos;
}

pos pos::operator+ (const double amount)
{
    pos amount_over = ratio();
    pos temp_pos = *this;

    temp_pos.x += amount * amount_over.x;
    temp_pos.y += amount * amount_over.y;

    return temp_pos;
}

pos& pos::operator+= (const pos amount)
{
    x += amount.x;
    y += amount.y;

    return *this;
}

pos& pos::operator+= (const double amount)
{
    pos amount_over = ratio();

    x += amount * amount_over.x;
    y += amount * amount_over.y;

    return *this;
}

pos pos::operator- (const pos amount)
{
    pos temp_pos = *this;
    temp_pos.x -= amount.x;
    temp_pos.y -= amount.y;

    return temp_pos;
}

pos pos::operator- (const double amount)
{
    pos amount_over = ratio();
    pos temp_pos = *this;

    temp_pos.x -= amount * amount_over.x;
    temp_pos.y -= amount * amount_over.y;

    return temp_pos;
}

pos& pos::operator-= (const pos amount)
{
    x -= amount.x;
    y -= amount.y;

    return *this;
}

pos& pos::operator-= (const double amount)
{
    pos amount_over = ratio();

    x -= amount * amount_over.x;
    y -= amount * amount_over.y;

    return *this;
}


pos pos::operator* (const pos amount)
{
    pos temp_pos = *this;
    temp_pos.x *= amount.x;
    temp_pos.y *= amount.y;

    return temp_pos;
}

pos pos::operator* (const double amount)
{
    pos temp_pos = *this;

    temp_pos.x *= amount;
    temp_pos.y *= amount;

    return temp_pos;
}

pos& pos::operator*= (const pos amount)
{
    x *= amount.x;
    y *= amount.y;

    return *this;
}

pos& pos::operator*= (const double amount)
{
    x *= amount;
    y *= amount;

    return *this;
}


pos pos::operator/ (const pos amount)
{
    pos temp_pos = *this;
    temp_pos.x /= amount.x;
    temp_pos.y /= amount.y;

    return temp_pos;
}

pos pos::operator/ (const double amount)
{
    pos temp_pos = *this;

    temp_pos.x /= amount;
    temp_pos.y /= amount;

    return temp_pos;
}

pos& pos::operator/= (const pos amount)
{
    x /= amount.x;
    y /= amount.y;

    return *this;
}

pos& pos::operator/= (const double amount)
{
    x /= amount;
    y /= amount;

    return *this;
}

bool pos::operator== (const pos& to_compare)
{
    if (x == to_compare.x && y == to_compare.y)
    {
        return true;
    }

    return false;
}

bool pos::operator!= (const pos& to_compare)
{
    if (*this == to_compare)
    {
        return false;
    }

    return true;
}

std::ostream& operator<< (std::ostream& os, const pos& convert)
{
    os << "X: " << convert.x << " Y: " << convert.y;
    return os;
}