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

#ifndef GANDIVA_REX_FULL_MATCH_H
#define GANDIVA_REX_FULL_MATCH_H

#include <string.h>

#include <unordered_map>
#include <vector>

#include "re2/re2.h"
#include "re2/stringpiece.h"
#include "types.h"

using std::string;
using kv_map = std::unordered_map<string, string>;

kv_map rex_full_match(const string& raw_message, const string& pattern_string,
                      bool in_valid, bool* out_valid) {
  // output is invalid.
  *out_valid = false;
  kv_map ret;
  if (in_valid) {
    re2::StringPiece input(raw_message);
    RE2 pattern(pattern_string);
    std::map<int, string> field_keys = pattern.CapturingGroupNames();
    field_keys.size();
    int keys_count = pattern.NumberOfCapturingGroups();
    string field_values[keys_count];
    std::vector<RE2::Arg> arg_vector;
    std::vector<RE2::Arg*> arg_prt_vector;
    for (int i = 0; i < keys_count; i++) {
      arg_vector.push_back(RE2::Arg(&field_values[i]));
    }
    for (auto& arg : arg_vector) {
      arg_prt_vector.push_back(&arg);
    }
    const RE2::Arg* matches[keys_count];
    std::move(arg_prt_vector.begin(), arg_prt_vector.end(), matches);
    bool is_matched = RE2::FullMatchN(input, pattern, matches, keys_count);
    if (is_matched) {
      for (int i = 0; i < keys_count; i++) {
        ret.insert(std::pair<string, string>(field_keys[i + 1], field_values[i]));
      }
      // output is valid.
      *out_valid = true;
      return ret;
    }
  }
  return ret;
}
#endif  // GANDIVA_REX_FULL_MATCH_H
