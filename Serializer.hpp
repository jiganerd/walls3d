//
//  Serializer.hpp
//  walls3d
//
//  Created by Brian Dolan on 5/21/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#ifndef Serializer_hpp
#define Serializer_hpp

#include <string>
#include <cstdint>

// this class is particular to the nature of values in this program - mainly, for
// floating point numbers, we expect to not have to worry about NaN or +/- infinity, and
// we don't need a great deal of precision after the decimal place - for these reasons,
// it is ok to convert between floating point numbers and 16.16 fixed point in (de)serialization
//
// this class intentionally does not make use of C++ streams in order to allow it to work
// on platforms that do not support them
class Serializer
{
public:
    // a 16.16 fixed-point representation of a floating point-number
    // this could moved out to a more fully-functional class at some point if useful
    class Fixed
    {
    public:
        Fixed(double number);
        Fixed(int32_t encoding);
        ~Fixed() = default;
        double Unfixed() const;
        int32_t GetEncoding() const { return encoding; }
        
        // a (public) constexpr version of this function is useful for static_asserts
        static constexpr double Unfixed(int32_t encoding)
        {
            return (static_cast<double>(encoding) / res);
        }
        
    private:
        int32_t encoding;
        
        // resolution of a fixed-point number (the amount that the LSB in the fractional part represents)
        static constexpr double res { static_cast<double>(static_cast<int32_t>(1) << 16) };
    };
    
    Serializer() = delete;
    ~Serializer() = delete;
    
    static std::string Ser(double n);
    static std::string Ser(int32_t n);
    static std::string Ser(uint32_t n);
    
    static double DeSerDouble(const uint8_t* bytes, size_t& offset);
    static int32_t DeSerInt(const uint8_t* bytes, size_t& offset);
    static uint32_t DeSerUint(const uint8_t* bytes, size_t& offset);
    
    static int32_t PeekInt(const uint8_t* bytes, size_t offset);
    
private:
    static std::string ByteToString(uint8_t byte);
    static void ToBytes(uint32_t n, uint8_t* bytes);
    static uint32_t FromBytes(const uint8_t bytes[]);
};

#endif /* Serializer_hpp */
