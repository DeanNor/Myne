
#include ".hpp/pos.hpp"

void pos::load(Loader* load)
{
    x = load->load_data<double>();
    y = load->load_data<double>();
}

void pos::save(Saver* save)
{
    save->save_data(x);
    save->save_data(y);
}

double pos::sum()
{
    return std::abs(x) + std::abs(y);
}

rad pos::direction()
{
    rad dir = pos(0,0).angle_to(*this);
    return dir;
}

pos pos::ratio()
{
    pos x_to_y = {0,0};

    const double scale = sum();

    x_to_y.x = x / scale;
    x_to_y.y = y / scale;

    return x_to_y;
}

double pos::len()
{
    return std::sqrt(std::pow(x,2) + std::pow(y,2));
}

pos pos::rotated(rad angle)
{
    if (angle != rad(0.0))
    {
        const double cosine = std::cos(angle);
        const double sine = std::sin(angle);

        pos temp_pos;

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
    return atan2(target.y - y,target.x - x);
}

pos pos::limited(double limit)
{
    const double len = std::abs(x) + std::abs(y);
    pos temp_pos = {0,0};

    if (len > limit)
    {
        const double ratio_over = (len) / limit;

        if (ratio_over > 0)
        {
            temp_pos.x = x / ratio_over;
            temp_pos.y = y / ratio_over;
        }
    }

    return temp_pos;
}

pos pos::scaled(pos start, pos end) const
{
    pos temp_pos = *this;

    temp_pos *= end / start;
    
    return temp_pos;
}

bool pos::within(const pos& min, const pos& max) const
{
    if (x < min.x || x > max.x)
    {
        return false;
    }

    if (y < min.y || y > max.y)
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
    const pos amount_over = ratio();
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
    const pos amount_over = ratio();

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
    const pos amount_over = ratio();
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
    const pos amount_over = ratio();

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