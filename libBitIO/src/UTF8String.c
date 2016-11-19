#include "../include/BitIO.h"

#ifdef __cplusplus
extern "C" {
#endif

    void       CheckBOM(UTF8String *String) {
        uint32_t BOMCheck = 0;

        for (uint8_t CodeUnit = 0; CodeUnit < 3; CodeUnit++) {
            BOMCheck += String->Graphemes[0]->CodeUnit[CodeUnit];
            BOMCheck << 8;
        }
        if (((BOMCheck & 0xFFFF00) >> 8) == UTF8String_BOM1_BE) {
            String->Endian = BOM1_BE;
        } else if (((BOMCheck & 0xFFFF00) >> 8) == UTF8String_BOM1_LE) {
            String->Endian = BOM1_LE;
        } else if ((BOMCheck & 0xFFFFFF) == UTF8String_BOM2_BE) {
            String->Endian = BOM2_BE;
        } else if ((BOMCheck & 0xFFFFFF) == UTF8String_BOM2_LE) {
            String->Endian = BOM2_LE;
        } else {
            String->Endian = NoBOM;
        }
    }

    /*
    bool       CodeUnitIsStartCodePoint(UTF8String *String, uint64_t StartCodeUnit) { // Should I add support for finding the closest previous Code Point?
        bool Truthiness = 0;
        for (uint64_t CodeUnit = StartCodeUnit; CodeUnit < String->StringSizeInCodeUnits; CodeUnit++) {
            if (((String->Graphemes[CodeUnit] & 0xE0) == 0xC0) || ((String->CodePoints[CodeUnit] & 0xE0) == 0xE0)) { // Confirmed as code point
                Truthiness = true;
            } else {
                Truthiness = false;
            }
        }
        return Truthiness;
    }
     */

    /*
    int8_t     CodePointSize(int8_t CodeUnit, uint64_t CodePointStartInCodeUnits) { // CodePointSize(UTF8String *String, uint64_t CodePointStartInCodeUnits)
        uint8_t CodeUnitsInCodePoint = 0;

        if ((CodeUnit & 0x80) == 0) { // ASCII
            CodeUnitsInCodePoint =  1;
        } else if ((CodeUnit & 0xE0) == 0x6) { // 1 more code unit, aka 2 total.
            CodeUnitsInCodePoint =  2;
        } else if ((CodeUnit & 0xF0) == 0xE) { // 2 more code units, aka 3 total.
            CodeUnitsInCodePoint =  3;
        } else if ((CodeUnit & 0xF8) == 0x1E) { // 3 more code units, aka 4 total
            CodeUnitsInCodePoint =  4;
        } else if ((CodeUnit & 0xC0) == 2) { // Continuation CU.
            CodeUnitsInCodePoint = -1;
        }

        return CodeUnitsInCodePoint;
    }
     */

    /*
    void       MeasureString(UTF8String *String) {
        uint64_t NumCodeUnits  = 0;
        uint64_t NumCodePoints = 0;

        for (uint64_t CodeUnit = 0; CodeUnit < UTF8String_MaxCodeUnits; CodeUnit++) {
            if ((String->CodePoints[CodeUnit] & 0x80) == 1) {
                NumCodeUnits  += 1;
                NumCodePoints += 1;
            } else if ((String->CodePoints[CodeUnit] & 0xF8) == 0x1F) {
                NumCodeUnits  += 1;
                NumCodePoints += 1;
            } else if ((String->CodePoints[CodeUnit] & 0xF0) == 0x0F) {
                NumCodeUnits  += 1;
                NumCodePoints += 1;
            } else if ((String->CodePoints[CodeUnit] & 0xE0) == 0x07) {
                NumCodeUnits  += 1;
                NumCodePoints += 1;
            }
        }
        String->StringSizeInCodePoints = NumCodePoints;
        String->StringSizeInCodeUnits  = NumCodeUnits;
    }
     */

    /*!
     Character2Remove has to be the first code point in a character to remove
     */
    void       RemoveSubString(UTF8String *OldString, UTF8String *NewString, UTF8String String2Remove, bool RemoveAll) {
        uint32_t CurrentCodePoint = 0, CurrentCodeUnit = 0, CodePointSize = 0;


        //if (CodeUnitIsStartCodePoint(OldString, ) == ) {

        //}
        for (uint64_t CodeUnit = 0; CodeUnit < OldString->StringSizeInCodeUnits; CodeUnit++) {
            uint8_t CodePointType = 0;
            if ((OldString->CodePoints[CodeUnit] & 0xF8) == 0xF0) { // First CodeUnit in a 4 byte CodePoint, 3 follow, + accents, maybe
                CurrentCodePoint += 1;
                CodePointType = FourCodeUnitsInCodePoint;
                if (OldString->CodePoints[CodeUnit] == String2Remove.CodePoints[CodeUnit]) {

                }
            } else if ((OldString->CodePoints[CodeUnit] & 0xF0) == 0xE0) { // First CodeUnit in a 3 byte CodePoint, 2 follow.
                CurrentCodePoint += 1;
                CodePointType = ThreeCodeUnitsInCodePoint;
                if (OldString->CodePoints[CodeUnit] == String2Remove.CodePoints[CodeUnit]) {

                }
            } else if ((OldString->CodePoints[CodeUnit] & 0xE0) == 0xC0) { // First CodeUnit in a 2 byte CodePoint, 1 follows.
                CurrentCodePoint += 1;
                CodePointType = TwoCodeUnitsInCodePoint;
                if (OldString->CodePoints[CodeUnit] == String2Remove.CodePoints[CodeUnit]) {

                }
            } else if ((OldString->CodePoints[CodeUnit] & 0x80) == 0x80) { // ASCII
                CurrentCodePoint += 1;
                CurrentCodeUnit += 1;
                CodePointType = OneCodeUnitInCodePoint;
                if (OldString->CodePoints[CodeUnit] == String2Remove.CodePoints[CodeUnit]) {

                }
            } else {
                // Not a UTF8 String, maybe it needs to be endian swapped?
            }
        }
    }

    /*
    UTF8String RemoveBOM(UTF8String *OldString, UTF8String *NewString, uint8_t BOMType) {
        if (BOMType == NoBOM) {
            return *OldString;
        } else if (BOMType == BOM1_BE) {
            // remove the first 2 bytes
            RemoveCharacter(OldString, NewString, BOM1_BE, false);
            OldString->StringSizeInCodePoints -= 2;
        } else if (BOMType == BOM1_LE) {
            // remove the first 2 bytes
            CopyUTF8String(OldString, NewString);
            RemoveCharacter(OldString, NewString, BOM1_LE, false);
            OldString->StringSizeInCodePoints -= 2;
        } else if (BOMType == BOM2_BE) {
            // remove the first 3 bytes
            RemoveCharacter(OldString, NewString, BOM2_BE, false);
            OldString->StringSizeInCodePoints -= 3;
        } else if (BOMType == BOM2_LE) {
            // remove the first 3 bytes
            RemoveCharacter(OldString, NewString, BOM2_LE, false);
            OldString->StringSizeInCodePoints -= 3;
        }

        return *OldString;
    }
     */

    UTF8String ConcatenateStrings(UTF8String *String1, UTF8String *String2, UTF8String *NewString) {
        uint64_t CumulativeSize = String1->StringSizeInCodePoints + String2->StringSizeInCodePoints;
        if (CumulativeSize > UTF8String_MaxCodeUnits) {
            // Error.
        } else {
            uint8_t String1BOM = CheckBOM(String1);
            uint8_t String2BOM = CheckBOM(String2);

            uint8_t NativeEndian = DetectNativeEndian();

            if ((NativeEndian == BigEndian) && (String1BOM != BigEndian)) {
                // Swap endian
            } else if ((NativeEndian == LittleEndian) && (String1BOM != LittleEndian)) {
                // Swap endian
            }
            // Wait, if String1 endian matches String2's endian, you're good

            uint32_t BOMCheck = 0;
            bool     BOMType  = NoBOM;

            for (uint8_t Byte = 0; Byte < 3; Byte++) {
                BOMCheck = String2->CodePoints[Byte] << 8;
            }
            if ((BOMCheck & 0xFFFF00) == UTF8String_BOM1_BE) {
                BOMType = BOM1_BE;
            } else if ((BOMCheck & 0xFFFF00) == UTF8String_BOM1_LE) {
                BOMType = BOM1_LE;
            } else if ((BOMCheck & 0xFFFFFF) == UTF8String_BOM2_BE) {
                BOMType = BOM2_BE;
            } else if ((BOMCheck & 0xFFFFFF) == UTF8String_BOM2_LE) {
                BOMType = BOM2_LE;
            } else {
                BOMType = NoBOM;
            }

            CopyUTF8String(String1, NewString);
            for (uint64_t CodePoint = String1->StringSizeInCodePoints + 1; CodePoint < CumulativeSize; CodePoint++) {
                NewString[CodePoint] = String2[CodePoint];
            }
            for (uint64_t CodePoint = 0; CodePoint < String2->StringSizeInCodePoints; CodePoint++) {
                //NewString->CharacterSize[CodePoint] = String2->CharacterSize[CodePoint];
            }
            NewString->StringSizeInCodeUnits += String2->StringSizeInCodeUnits;
            NewString->StringSizeInCodePoints += String2->StringSizeInCodePoints;
        }
        return *NewString;
    }

    bool       CompareUTF8Strings(UTF8String *String1, UTF8String *String2, bool CaseSensitive) {
        if (CaseSensitive == false) {
            // compare code points
        } else {
            //
        }
        return 1;
    }

    /*!
     @abstract    "Replace String2Find with String2Replace in String; if ReplaceAll = false only the first occurrence is replaced".
     */
    UTF8String ReplaceCharacter(UTF8String *String, UTF8String String2Find, UTF8String String2Replace, bool ReplaceAll) {
        return *String;
    }

    uint64_t   LocateGrapheme(UTF8String *String, UTF8String *Character2Locate, bool StartAtEnd) {
        uint64_t Position = 0;

        if (StartAtEnd == true) {
            for (uint64_t Character = String->StringSizeInCodeUnits; Character > 0; Character--) {
                if (CompareStrings(String, Character2Locate, false) == 0) {
                    Position = Character;
                }
            }
        } else {
            for (uint64_t Character = 0; Character < String->StringSizeInCodeUnits; Character++) {
                if (String->CodePoints[Character] == Character2Locate) {
                    Position = Character;
                }
            }
        }

        return Position;
    }

    /*!
     @abstract           "Seperates String based on Seperator".
     */
    UTF8String SeperateString(UTF8String *String, UTF8String Seperator) { // va_list Arguments
        return *String;
    }

    UTF8String NormalizeString(UTF8String *String2Normalize) {
        return 0;
    }

    /*
     @param      SortTable       "Submit the table to sort the string with".
     */
    /*
    UTF8String SortString(UTF8String *String2Sort, uint8_t *SortTable) { // sort numbers first, followed by capital letters then lower case, then the rest of Unicode.
        NormalizeString(String2Sort);
        // Compare the normalized string to the table in levels as specified http://www.unicode.org/reports/tr10/#Main_Algorithm

        return *String2Sort;
    }
     */

    void SeperateUTF8String(UTF8String *String2Split, UTF8String *Seperator) {
        // tl;dr scan each grapeme for the character to split on, when found, split the string
    }

    void ReadUTF8String(BitInput *BitI, UTF8String *String, uint64_t Graphemes2Read) {
        uint8_t Byte = 0;

        Byte = ReadBits(BitI, 8);
        if ((Byte & 0x80) == ) {
            <#statements#>
        }



        // Count UTF-8 characters.
        if ((Characters2Read == 0) || (Characters2Read == 0xFFFFFFFFFFFFFFFF)) {
            // Read until you come across a new line.

        } else {
            // Count each character until we reach Characters2Read.
            for (uint64_t CodeUnit = 0; CodeUnit < Characters2Read; CodeUnit++) {
                TempCodeUnit = ReadBits(BitI, 8);
                if (CodePointSize(TempCodeUnit, 1) == -1) {
                    // Continuation byte
                } else {
                    // Start of code point, increment
                }

                String->CodePoints[CodeUnit] = TempCodeUnit;
            }
        }
    }
    
    void WriteUTF8String(BitOutput *BitO, UTF8String *String2Write) {
        
    }
    
    uint64_t CountGraphemes(uint64_t StartUnit, size_t NumCodeUnits, uint8_t *CodeUnits[]) {
        uint8_t Data = 0;
        for (uint64_t CodeUnit = StartUnit; CodeUnit < NumCodeUnits; CodeUnit++) {
            Data = CodeUnits[CodeUnit];
            
        }
        return 0;
    }
    
    UTF8String CreateUTF8String(size_t NumGraphemes, uint8_t *CodeUnits[]) { // call memset with 0xFF, because that's an invalid value in UTF AND ASCII
                                                                             // In order to create a string, I need to
        
        // Scan through the code units, when you find the start of a new one, go to the next grapheme.
        
        uint64_t NumCodeUnits = sizeof(CodeUnits);
        UTF8String *String2Create = malloc(sizeof(UTF8String * NumCodeUnits));
        if (NumGraphemes <= MaxGraphemes) {
            memset(String2Create, 0xFF, sizeof(String2Create));
            for (uint64_t Grapheme = 0; Grapheme < NumGraphemes; Grapheme++) {
                for (uint8_t CodeUnit = 0; CodeUnit < MaxCodeUnits; CodeUnit++) {
                    String2Create->Grapheme[Grapheme][CodeUnit];
                }
            }
        }
        return String2Create;
    }
    
    void DestroyUTF8String(UTF8String *String2Destroy) {
        for (uint64_t Grapheme = 0; Grapheme < String2Destroy->Graphemes; Grapheme++) {
            free(String2Destroy->Graphemes);
        }
        free(String2Destroy);
    }
    
    
    // How would my theoretical string type work? Well I want to support at least 4096 graphemes per string, if not 65535.
    // IF we were to have 2 levels, the first wouldcontain an array of all the codeunits in each grapheme.
    // The second would be a rope of each character that compromises the string?
    // But I'd also like to support seperating strings based on the grapheme, how would that fit in?
    // Well, if I did it this way, we could index a string in 0(n) time, like with ASCII strings... My only concern is how much memory would a string of this size take?
    // Let's say for example that I have 4096 graphemes, all 4 code units in size. in ASCII this would take 16,384 bytes.
    // In this structure, it would take 4096 pointers, + 4096 32 bit graphemes, or 49,152 bytes, aka 3 times more memory...
    
    // To make a Rope datastructure, I need to base it on a "linked list" aka, a 2D array where the first dimension is the value, and the second is a pointer to the next element.
    
    
    
#ifdef __cplusplus
}
#endif