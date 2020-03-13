// Copyright (C) 2017-2018 Dremio Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "rex_full_match.h"

#include <gtest/gtest.h>

namespace gandiva {

TEST(TestRexFullMatch, rex_full_match) {
  bool in_valid = true;
  bool out_valid = false;
  kv_map ret;
  kv_map bench_mark;
  bench_mark.insert(std::pair<string, string>("aos", "Windows NT"));
  bench_mark.insert(std::pair<string, string>("abytes", "97106"));
  bench_mark.insert(std::pair<string, string>("aversion", "HTTP/1.1"));
  bench_mark.insert(std::pair<string, string>("aurl", "/apache-log/access.log"));

  string raw_message =
      "107.173.176.148 - - [13/Dec/2015:05:11:56 +0100] \"GET /apache-log/access.log "
      "HTTP/1.1\" 200 97106 \"http://www.almhuette-raith.at/\" \"Mozilla/5.0 (Windows NT "
      "6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Iron/29.0.1600.1 "
      "Chrome/29.0.1600.1 Safari/537.36\" \"-\"";
  string pattern_string =
      ".+ (?P<aurl>/[0-9a-zA-Z\\./\\-\\_\\~]*) +(?P<aversion>HTTP/[0-9]+\\.[0-9]+).+ "
      "[0-9]{3} +(?P<abytes>[0-9\\-]+) .+\\((?P<aos>Macintosh|Windows "
      "NT|iPhone|compatible).+";

  // all parameters valid
  ret = rex_full_match(raw_message, pattern_string, in_valid, &out_valid);
  EXPECT_EQ(ret, bench_mark);
  EXPECT_EQ(out_valid, true);

  // in_valid is false
  in_valid = false;
  ret = rex_full_match(raw_message, pattern_string, in_valid, &out_valid);
  EXPECT_EQ(ret, kv_map());
  EXPECT_EQ(out_valid, false);

  // raw_message is less than the pattern
  in_valid = true;
  ret = rex_full_match(
      "107.173.176.148 - - [13/Dec/2015:05:11:56 +0100] \"GET /apache-log/access.log "
      "HTTP/1.1\" 200 97106 \"http://www.almhuette-raith.at/\"",
      pattern_string, in_valid, &out_valid);
  EXPECT_EQ(ret, kv_map());
  EXPECT_EQ(out_valid, false);

  // pattern is less than the raw_message
  in_valid = true;
  ret = rex_full_match(
      raw_message,
      ".+ (?P<aurl>/[0-9a-zA-Z\\./\\-\\_\\~]*) +(?P<aversion>HTTP/[0-9]+\\.[0-9]+).+ "
      "[0-9]{3} +(?P<abytes>[0-9\\-]+) .+\\((?P<aos>Macintosh|Windows "
      "NT|iPhone|compatible)",
      in_valid, &out_valid);
  EXPECT_EQ(ret, kv_map());
  EXPECT_EQ(out_valid, false);
}

}  // namespace gandiva
