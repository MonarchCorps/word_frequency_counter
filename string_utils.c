//
// Created by David Okocha on 06/05/2026.
//

#include "string_utils.h"

#include <ctype.h>

void to_lowercase(char* str)
{
    for (int i = 0; str[i]; i++)
    {
        // (unsigned char) cast reinterprets bit pattern as unsigned first
        // so a char that was -28 becomes 228 — valid input to tolower
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}
