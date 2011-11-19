/***************************************************************************
 *   Copyright (C) 2010~2010 by Margaret Wang                              *
 *   pipituliuliu@gmail.com                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.              *
 ***************************************************************************/

#include "tables.h"
#include "eim.h"

int checkIfCorrectionNeeded(const char *buffer)
{
    char c1 = *buffer;
    char c2 = *(buffer + 1);
    return c1 == 'n' && (c2 != 'a' && c2 != 'i' && c2 != 'u'
                         && c2 != 'e' && c2 != 'o' && c2 != 'n' && c2 != 'y');
}


int FcitxAnthyConvertRomajiToKana(struct _FcitxAnthy *anthy)
{
    char *input_buffer = anthy->input_state.input_buffer;
    int input_count = anthy->input_state.input_count;
    char *romaji_buffer = anthy->input_state.romaji_buffer;
    int romaji_count = anthy->input_state.romaji_count;

    if (checkIfCorrectionNeeded(romaji_buffer)) {
        //apply correction rule in input
        strcpy(input_buffer + input_count, "ん");
        input_count += strlen("ん");
        input_buffer[input_count] = '\0';

        //update romaji buffer
        romaji_buffer[0] = romaji_buffer[1];
        romaji_buffer[1] = '\0';
        romaji_count = 1;
    } else {
        const char *kana_string = NULL, *extra_romaji_string = NULL;
        FcitxAnthyLookupKanaForRomaji(anthy, romaji_buffer, &kana_string, &extra_romaji_string);

        if (kana_string) {
            strcpy(input_buffer + input_count, kana_string);
            input_count += strlen(kana_string);
            if (extra_romaji_string) {
                strcpy(romaji_buffer, extra_romaji_string);
                romaji_count = strlen(romaji_buffer);
            } else {
                romaji_buffer[0] = '\0';
                romaji_count = 0;
            }
        } else {
            // todo: lookup symbol table
        }
    }
    anthy->input_state.input_count = input_count;
    anthy->input_state.romaji_count = romaji_count;

    return 0;
}