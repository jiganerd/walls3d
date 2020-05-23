//
//  Serializer.cpp
//  walls3d
//
//  Created by Brian Dolan on 5/21/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#include <iomanip>
#include <sstream>
#include "Serializer.hpp"

Serializer::Fixed::Fixed(double number):
    // TODO: round() would be better than truncation
    // TODO: assert n is not out of bounds
    encoding{static_cast<int32_t>(number * res)}
{
}

Serializer::Fixed::Fixed(int32_t encoding):
    encoding{encoding}
{
}

double Serializer::Fixed::Unfixed() const
{
    return Unfixed(encoding);
}

std::string Serializer::Ser(double n)
{
    return Ser(Fixed(n).GetEncoding());
}

std::string Serializer::Ser(int32_t n)
{
    return Ser(static_cast<uint32_t>(n));
}

std::string Serializer::Ser(uint32_t n)
{
    uint8_t bytes[4];
    ToBytes(n, bytes);
    std::string ret;
    for (auto b : bytes)
        ret += ByteToString(b);
    return ret;
}

double Serializer::DeSerDouble(const uint8_t* bytes, size_t& offset)
{
    return Fixed(DeSerInt(bytes, offset)).Unfixed();
}

int32_t Serializer::DeSerInt(const uint8_t* bytes, size_t& offset)
{
    return static_cast<int32_t>(DeSerUint(bytes, offset));
}

uint32_t Serializer::DeSerUint(const uint8_t* bytes, size_t& offset)
{
    uint32_t ret = FromBytes(bytes + offset);
    offset += 4;
    return ret;
}

int32_t Serializer::PeekInt(const uint8_t* bytes, size_t offset)
{
    return FromBytes(bytes + offset);
}

std::string Serializer::ByteToString(uint8_t byte)
{
    std::stringstream stream;
    stream << "0x" << std::setfill('0') << std::setw(2) << std::hex << unsigned(byte) << ", ";
    return stream.str();
}

void Serializer::ToBytes(uint32_t n, uint8_t* bytes)
{
    bytes[0] = (n >> 24) & 0xFF;
    bytes[1] = (n >> 16) & 0xFF;
    bytes[2] = (n >>  8) & 0xFF;
    bytes[3] =  n        & 0xFF;
}

uint32_t Serializer::FromBytes(const uint8_t bytes[])
{
    return
        (static_cast<uint32_t>(bytes[3]) << 0)  |
        (static_cast<uint32_t>(bytes[2]) << 8)  |
        (static_cast<uint32_t>(bytes[1]) << 16) |
        (static_cast<uint32_t>(bytes[0]) << 24);
}