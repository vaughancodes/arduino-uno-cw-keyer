const char* const MORSE_CODE_TABLE[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",  // A-J
  "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",    // K-T
  "..-", "...-", ".--", "-..-", "-.--", "--..",                           // U-Z
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."  // 0-9
};

const char CHAR_TABLE[] = {
  'A','B','C','D','E','F','G','H','I','J',
  'K','L','M','N','O','P','Q','R','S','T',
  'U','V','W','X','Y','Z',
  '0','1','2','3','4','5','6','7','8','9'
};

const int MORSE_TABLE_SIZE = sizeof(CHAR_TABLE) / sizeof(CHAR_TABLE[0]);

char decodeMorse(const char* morse) {
  for (int i = 0; i < MORSE_TABLE_SIZE; i++) {
    if (strcmp(morse, MORSE_CODE_TABLE[i]) == 0) {
      return CHAR_TABLE[i];
    }
  }
  return '?'; // Unknown symbol
}