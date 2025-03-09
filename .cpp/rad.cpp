
#include "rad.hpp"

#define PI 3.14159
double constrain_rad(double amount)
{
    if (amount > PI || amount <= -PI)
    {
        return PI - fmod(amount, PI);
    }

    return amount;
}

rad::rad(double val) : radian(constrain_rad(val))
{

}

rad::rad()
{
    radian = 0;
}

rad::operator double()
{
    return radian;
}

// ----- ?
bool rad::operator> (rad compare)
{
    return radian > compare.radian;
}

bool rad::operator< (rad compare)
{
    return radian < compare.radian;
}

bool rad::operator>= (rad compare)
{
    return radian >= compare.radian;
}

bool rad::operator<= (rad compare)
{
    return radian <= compare.radian;
}

bool rad::operator== (rad compare)
{
    return radian == compare.radian;
}

// ----- +
rad rad::operator+ (rad amount)
{
    return amount.radian + radian;
}

long double rad::operator+ (long double amount)
{
    return constrain_rad((long double)radian + amount);
}

double rad::operator+ (double amount)
{
    return constrain_rad(radian + amount);
}

float rad::operator+ (float amount)
{
    return constrain_rad((float)radian + amount);
}

unsigned long long rad::operator+ (unsigned long long amount)
{
    return constrain_rad((unsigned long long)radian + amount);
}

unsigned long int rad::operator+ (unsigned long int amount)
{
    return constrain_rad((unsigned long int)radian + amount);
}

long long rad::operator+ (long long amount)
{
    return constrain_rad((long long)radian + amount);
}

long rad::operator+ (long amount)
{
    return constrain_rad((long)radian + amount);
}

int rad::operator+ (int amount)
{
    return constrain_rad((int)radian + amount);
}

short rad::operator+ (short amount)
{
    return constrain_rad((short)radian + amount);
}

// ----- +=
rad& rad::operator+= (rad amount)
{
    radian = radian + amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator+= (long double amount)
{
    radian = (long double) radian + amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator+= (double amount)
{
    radian = (double) radian + amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator+= (float amount)
{
    radian = (float) radian + amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator+= (unsigned long long amount)
{
    radian = (unsigned long long) radian + amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator+= (unsigned long int amount)
{
    radian = (unsigned long int) radian + amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator+= (long long amount)
{
    radian = (long long) radian + amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator+= (long amount)
{
    radian = (long) radian + amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator+= (int amount)
{
    radian = (int) radian + amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator+= (short amount)
{
    radian = (short) radian + amount;

    radian = constrain_rad(radian);

    return *this;
}

// ----- -
rad rad::operator- (rad amount)
{
    return amount.radian - radian;
}

long double rad::operator- (long double amount)
{
    return constrain_rad((long double)radian - amount);
}

double rad::operator- (double amount)
{
    return constrain_rad(radian - amount);
}

float rad::operator- (float amount)
{
    return constrain_rad((float)radian - amount);
}

unsigned long long rad::operator- (unsigned long long amount)
{
    return constrain_rad((unsigned long long)radian - amount);
}

unsigned long int rad::operator- (unsigned long int amount)
{
    return constrain_rad((unsigned long int)radian - amount);
}

long long rad::operator- (long long amount)
{
    return constrain_rad((long long)radian - amount);
}

long rad::operator- (long amount)
{
    return constrain_rad((long)radian - amount);
}

int rad::operator- (int amount)
{
    return constrain_rad((int)radian - amount);
}

short rad::operator- (short amount)
{
    return constrain_rad((short)radian - amount);
}

// ----- -=
rad& rad::operator-= (rad amount)
{
    radian = radian - amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator-= (long double amount)
{
    radian = (long double) radian - amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator-= (double amount)
{
    radian = (double) radian - amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator-= (float amount)
{
    radian = (float) radian - amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator-= (unsigned long long amount)
{
    radian = (unsigned long long) radian - amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator-= (unsigned long int amount)
{
    radian = (unsigned long int) radian - amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator-= (long long amount)
{
    radian = (long long) radian - amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator-= (long amount)
{
    radian = (long) radian - amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator-= (int amount)
{
    radian = (int) radian - amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator-= (short amount)
{
    radian = (short) radian - amount;

    radian = constrain_rad(radian);

    return *this;
}

// ----- *
rad rad::operator* (rad amount)
{
    return amount.radian * radian;
}

long double rad::operator* (long double amount)
{
    return constrain_rad((long double)radian * amount);
}

double rad::operator* (double amount)
{
    return constrain_rad(radian * amount);
}

float rad::operator* (float amount)
{
    return constrain_rad((float)radian * amount);
}

unsigned long long rad::operator* (unsigned long long amount)
{
    return constrain_rad((unsigned long long)radian * amount);
}

unsigned long int rad::operator* (unsigned long int amount)
{
    return constrain_rad((unsigned long int)radian * amount);
}

long long rad::operator* (long long amount)
{
    return constrain_rad((long long)radian * amount);
}

long rad::operator* (long amount)
{
    return constrain_rad((long)radian * amount);
}

int rad::operator* (int amount)
{
    return constrain_rad((int)radian * amount);
}

short rad::operator* (short amount)
{
    return constrain_rad((short)radian * amount);
}

// ----- *=
rad& rad::operator*= (rad amount)
{
    radian = radian * amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator*= (long double amount)
{
    radian = (long double) radian * amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator*= (double amount)
{
    radian = (double) radian * amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator*= (float amount)
{
    radian = (float) radian * amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator*= (unsigned long long amount)
{
    radian = (unsigned long long) radian * amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator*= (unsigned long int amount)
{
    radian = (unsigned long int) radian * amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator*= (long long amount)
{
    radian = (long long) radian * amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator*= (long amount)
{
    radian = (long) radian * amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator*= (int amount)
{
    radian = (int) radian * amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator*= (short amount)
{
    radian = (short) radian * amount;

    radian = constrain_rad(radian);

    return *this;
}

// ----- /
rad rad::operator/ (rad amount)
{
    return amount.radian / radian;
}

long double rad::operator/ (long double amount)
{
    return constrain_rad((long double)radian / amount);
}

double rad::operator/ (double amount)
{
    return constrain_rad(radian / amount);
}

float rad::operator/ (float amount)
{
    return constrain_rad((float)radian / amount);
}

unsigned long long rad::operator/ (unsigned long long amount)
{
    return constrain_rad((unsigned long long)radian / amount);
}

unsigned long int rad::operator/ (unsigned long int amount)
{
    return constrain_rad((unsigned long int)radian / amount);
}

long long rad::operator/ (long long amount)
{
    return constrain_rad((long long)radian / amount);
}

long rad::operator/ (long amount)
{
    return constrain_rad((long)radian / amount);
}

int rad::operator/ (int amount)
{
    return constrain_rad((int)radian / amount);
}

short rad::operator/ (short amount)
{
    return constrain_rad((short)radian / amount);
}

// ----- /=
rad& rad::operator/= (rad amount)
{
    radian = radian / amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator/= (long double amount)
{
    radian = (long double) radian / amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator/= (double amount)
{
    radian = (double) radian / amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator/= (float amount)
{
    radian = (float) radian / amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator/= (unsigned long long amount)
{
    radian = (unsigned long long) radian / amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator/= (unsigned long int amount)
{
    radian = (unsigned long int) radian / amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator/= (long long amount)
{
    radian = (long long) radian / amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator/= (long amount)
{
    radian = (long) radian / amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator/= (int amount)
{
    radian = (int) radian / amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator/= (short amount)
{
    radian = (short) radian / amount;

    radian = constrain_rad(radian);

    return *this;
}