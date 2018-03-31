#include "RunLengthEncoder.hpp"
#include <smile/Exception.hpp>
#include <algorithm>
#include <assert.h>

namespace
{

const size_t ESCAPE_SIZE = 1;
const size_t REPEATER_SIZE = 2;
const size_t COUNT_SIZE = 2;
const size_t REPEATER_RECORD_SIZE = ESCAPE_SIZE + REPEATER_SIZE + COUNT_SIZE;
const size_t ESCAPE_RECORD_SIZE = ESCAPE_SIZE + ESCAPE_SIZE;

}

namespace smile
{

std::vector<uint8_t> RunLengthEncoder::decode(const std::vector<uint8_t>& bytes, uint8_t escape)
{
    std::vector<uint8_t> output;
    for (std::vector<uint8_t>::const_iterator itor = bytes.begin();
         itor != bytes.end();
         itor++)
    {
        if (*itor == escape)
        {
            assert(ESCAPE_SIZE == 1);
            if ((itor + ESCAPE_SIZE) == bytes.end())
                throw InvalidStateException(__FILE__, __LINE__, "Found escape at end of sequence");
            if (*(itor + ESCAPE_SIZE) == escape)
            {
                output.push_back(escape);
                itor++;
            }
            else
            {
                if (((itor - bytes.begin()) + REPEATER_SIZE + COUNT_SIZE) < bytes.size())
                {
                    uint8_t first = *(itor + ESCAPE_SIZE);
                    uint8_t second = *(itor + ESCAPE_SIZE + 1);
                    uint16_t repeatedCount = (*(itor + ESCAPE_SIZE + REPEATER_SIZE) << 8) +
                        (*(itor + ESCAPE_SIZE + REPEATER_SIZE + 1));
                    for (uint16_t i = 0; i < repeatedCount; i++)
                    {
                        output.push_back(first);
                        output.push_back(second);
                    }
                    itor += REPEATER_RECORD_SIZE - 1;
                }
                else
                {
                    throw InvalidStateException(__FILE__, __LINE__, "Incomplete repeater record");
                }
            }
        }
        else
        {
            output.push_back(*itor);
        }
    }
    return output;
}

std::vector<uint8_t> RunLengthEncoder::encode(const std::vector<uint8_t>& bytes, uint8_t escape)
{
    std::vector<uint8_t> output;
    for (std::vector<uint8_t>::const_iterator itor = bytes.begin();
         itor != bytes.end();
         itor++)
    {
        if (*itor == escape)
        {
            output.push_back(escape);
            output.push_back(escape);
        }
        else if ((itor + 1) == bytes.end())
        {
            output.push_back(*itor);
        }
        else if (*(itor + 1) == escape)
        {
            output.push_back(*itor);
            output.push_back(escape);
            output.push_back(escape);
            itor++;
        }
        else
        {
            std::vector<uint8_t>::const_iterator localItor = itor;
            uint16_t repeatedCount = 1;
            uint8_t first = *itor;
            uint8_t second = *(itor + 1);
            itor += 2;
            while (itor != bytes.end() &&
                   (itor + 1) != bytes.end() &&
                   *itor == first &&
                   *(itor + 1) == second &&
                   repeatedCount < 65535)
            {
                repeatedCount++;
                itor += 2;
            }
            if ((repeatedCount * REPEATER_SIZE) >= (REPEATER_RECORD_SIZE * 2))
            {
                output.push_back(escape);
                output.push_back(first);
                output.push_back(second);
                output.push_back((repeatedCount >> 8) & 0xff);
                output.push_back(repeatedCount & 0xff);
            }
            else
            {
                output.insert(output.end(), localItor, itor);
            }
            itor--;
        }
    }
    return output;
}

}
