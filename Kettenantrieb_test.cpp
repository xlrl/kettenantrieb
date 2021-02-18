#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <vector>

// here some stub functions (we do not have the actual
// arduino library)

enum
{
    INPUT = 0,
    OUTPUT = 1
};

enum
{
    LOW = 0,
    HIGH = 1,
};

enum
{
    A0 = 0,
    A1,
    AD_COUNT
};

enum
{
    PIN_COUNT = 11
};

struct ArduinoStub
{
    std::vector<unsigned> ad = std::vector<unsigned>(AD_COUNT);
    std::vector<unsigned> pwm = std::vector<unsigned>(PIN_COUNT);
    std::vector<unsigned> outputs = std::vector<unsigned>(PIN_COUNT);
    std::vector<unsigned> pinModes = std::vector<unsigned>(PIN_COUNT);

    ArduinoStub()
    {
        // all pins are inputs by default
        for (unsigned i=0; i < PIN_COUNT; i++)
        {
            pinModes[i] = INPUT;
        }
    }
};

static struct ArduinoStub m_ArduinoStub;

static void pinMode(unsigned pin, unsigned mode)
{
    m_ArduinoStub.pinModes[pin] = mode;
}

static int analogRead(unsigned ad)
{
    return m_ArduinoStub.ad[ad];
}

static void analogWrite(unsigned pin, unsigned pwm)
{
    REQUIRE(m_ArduinoStub.pinModes[pin] == OUTPUT);

    m_ArduinoStub.pwm[pin] = pwm;
}

static void digitalWrite(unsigned pin, unsigned value)
{
    m_ArduinoStub.outputs[pin] = value;
}

/**
 * Re-maps a number from one range to another.
 * That is, a value of fromLow would get mapped to toLow,
 * a value of fromHigh to toHigh, values in-between to
 * values in-between, etc.
 *
 * @param value     the number to map
 * @param fromLow   the lower bound of the value’s current range.
 * @param fromHigh  the upper bound of the value’s current range.
 * @param toLow     the lower bound of the value’s target range.
 * @param toHigh    the upper bound of the value’s target range.
 */
static int my_map(int value, int fromLow, int fromHigh, int toLow, int toHigh)
{
    int fromDelta = fromHigh - fromLow;
    int toDelta = toHigh - toLow;

    int retval = (value - fromLow) * toDelta;
    retval /= fromDelta;

    retval += toLow;

    return retval;
}

extern "C" {
// we need to rename some "map" so they don't
// clash with existing name
#define map my_map

// include the actual source code as header
#undef map
}

TEST_CASE("test my_map (map implementation)")
{
    SECTION("fromDelta positive, toDelta positive")
    {
        REQUIRE(my_map(10, 10, 20, 100, 200) == 100);
        REQUIRE(my_map(20, 10, 20, 100, 200) == 200);
    }

    SECTION("fromDelta negative, toDelta positive")
    {
        REQUIRE(my_map(10, 20, 10, 100, 200) == 200);
        REQUIRE(my_map(20, 20, 10, 100, 200) == 100);
    }

    SECTION("fromDelta positive, toDelta negative")
    {
        REQUIRE(my_map(10, 10, 20, 200, 100) == 200);
        REQUIRE(my_map(20, 10, 20, 200, 100) == 100);
    }

    SECTION("fromDelta negative, toDelta negative")
    {
        REQUIRE(my_map(10, 20, 10, 200, 100) == 100);
        REQUIRE(my_map(20, 20, 10, 200, 100) == 200);
    }
}
