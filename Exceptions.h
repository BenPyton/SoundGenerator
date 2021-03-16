/*
 * Copyright 2021 Benoit Pelletier
 *
 * This file is part of Sound Generator.
 *
 * Sound Generator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Sound Generator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Sound Generator.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <QException>

// Used to mark a function as not already implemented, but will be i a future
class NotImplementedException : public QException
{
public:
    virtual void raise() const override { throw *this; }
    virtual NotImplementedException* clone() const override { return new NotImplementedException(*this); }
    virtual const char* what() const override { return "Function is not implemented"; }
};

// Used to announce an error in one of the arguments passed to a function
class ArgumentException : public QException
{
public:
    ArgumentException(std::string _message): m_message(_message) {}
    ArgumentException(const ArgumentException& _other): m_message(_other.m_message) {}

    virtual void raise() const override { throw *this; }
    virtual ArgumentException* clone() const override { return new ArgumentException(*this); }
    virtual const char* what() const override { return m_message.c_str(); }
private:
    std::string m_message;
};

#endif // EXCEPTIONS_H
