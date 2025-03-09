
#pragma once

#include <cmath>

struct rad
{
public:
    double radian = 0;

    rad(double val);

    rad();

    operator double();

    // ----- ?
    bool operator> (rad compare);

    bool operator< (rad compare);

    bool operator>= (rad compare);

    bool operator<= (rad compare);

    bool operator== (rad compare);

    // ----- +
    rad operator+ (rad amount);

    long double operator+ (long double amount);

    double operator+ (double amount);

    float operator+ (float amount);

    unsigned long long operator+ (unsigned long long amount);

    unsigned long int operator+ (unsigned long int amount);

    long long operator+ (long long amount);

    long operator+ (long amount);

    int operator+ (int amount);

    short operator+ (short amount);

    // ----- +=
    rad& operator+= (rad amount);

    rad& operator+= (long double amount);

    rad& operator+= (double amount);

    rad& operator+= (float amount);

    rad& operator+= (unsigned long long amount);

    rad& operator+= (unsigned long int amount);

    rad& operator+= (long long amount);

    rad& operator+= (long amount);

    rad& operator+= (int amount);

    rad& operator+= (short amount);

    // ----- -
    rad operator- (rad amount);

    long double operator- (long double amount);

    double operator- (double amount);

    float operator- (float amount);

    unsigned long long operator- (unsigned long long amount);

    unsigned long int operator- (unsigned long int amount);

    long long operator- (long long amount);

    long operator- (long amount);

    int operator- (int amount);

    short operator- (short amount);

    // ----- -=
    rad& operator-= (rad amount);

    rad& operator-= (long double amount);

    rad& operator-= (double amount);

    rad& operator-= (float amount);

    rad& operator-= (unsigned long long amount);

    rad& operator-= (unsigned long int amount);

    rad& operator-= (long long amount);

    rad& operator-= (long amount);

    rad& operator-= (int amount);

    rad& operator-= (short amount);

    // ----- *
    rad operator* (rad amount);

    long double operator* (long double amount);

    double operator* (double amount);

    float operator* (float amount);

    unsigned long long operator* (unsigned long long amount);

    unsigned long int operator* (unsigned long int amount);

    long long operator* (long long amount);

    long operator* (long amount);

    int operator* (int amount);

    short operator* (short amount);

    // ----- *=
    rad& operator*= (rad amount);

    rad& operator*= (long double amount);

    rad& operator*= (double amount);

    rad& operator*= (float amount);

    rad& operator*= (unsigned long long amount);

    rad& operator*= (unsigned long int amount);

    rad& operator*= (long long amount);

    rad& operator*= (long amount);

    rad& operator*= (int amount);

    rad& operator*= (short amount);

    // ----- /
    rad operator/ (rad amount);

    long double operator/ (long double amount);

    double operator/ (double amount);

    float operator/ (float amount);

    unsigned long long operator/ (unsigned long long amount);

    unsigned long int operator/ (unsigned long int amount);

    long long operator/ (long long amount);

    long operator/ (long amount);

    int operator/ (int amount);

    short operator/ (short amount);

    // ----- /=
    rad& operator/= (rad amount);

    rad& operator/= (long double amount);

    rad& operator/= (double amount);

    rad& operator/= (float amount);

    rad& operator/= (unsigned long long amount);

    rad& operator/= (unsigned long int amount);

    rad& operator/= (long long amount);

    rad& operator/= (long amount);

    rad& operator/= (int amount);

    rad& operator/= (short amount);
};