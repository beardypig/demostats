/*
 *  Copyright (c) 2018 beardypig <beardypig@protonmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtest/gtest.h>
#include <string>
#include <hl2demo/dstream.h>

using namespace hl2demo;

TEST(DStream, BitSingleRead) {
    std::string data = "\x01";
    auto s = dstream(data);
    EXPECT_EQ(s.ulong(1), 0x01);
}

TEST(DStream, BitSingleReadTwice) {
    std::string data = "\x02";
    auto s = dstream(data);
    EXPECT_EQ(s.ulong(1), 0x00);
    EXPECT_EQ(s.ulong(1), 0x01);
}

TEST(DStream, MutiBitRead) {
    std::string data = "\x01\x06";  // 0000 0001 0000 0110
    auto s = dstream(data);
    EXPECT_EQ(s.ulong(9), 0x01);
    EXPECT_EQ(s.ulong(2), 0x03);
}

TEST(DStream, ReadBytes) {
    std::string data = "test";
    auto s = dstream(data);
    std::vector<char> result(4);
    s.read(&result[0], 4);
    std::string r = std::string(result.begin(), result.end());
    EXPECT_EQ(r, "test");
}

TEST(DStream, ReadBytesUnaligned) {
    std::string data = "\xe9\xca\xe6\xe8\x00\x00\x00\x00\x00";
    std::vector<char> result(4);

    auto s = dstream(data);
    EXPECT_EQ(s.ulong(1), 1);
    s.read(&result[0], 4);
    std::string r = std::string(result.begin(), result.end());
    EXPECT_EQ(r, "test");
}

TEST(DStream, ReadBytesAligned) {
    std::string data = "\x44test";
    std::vector<char> result(4);

    auto s = dstream(data);
    EXPECT_EQ(s.ulong(4), 4);
    EXPECT_EQ(s.ulong(4), 4);
    s.read(&result[0], 4);
    std::string r = std::string(result.begin(), result.end());
    EXPECT_EQ(r, "test");
}
