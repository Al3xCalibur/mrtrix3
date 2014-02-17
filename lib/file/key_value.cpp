#include <fstream>
#include "file/key_value.h"


namespace MR
{
  namespace File
  {

    void KeyValue::open (const std::string& file, const char* first_line)
    {
      filename.clear();
      DEBUG ("reading key/value file \"" + file + "\"...");

      in.open (file.c_str(), std::ios::in | std::ios::binary);
      if (!in) 
        throw Exception ("failed to open key/value file \"" + file + "\": " + strerror (errno));
      if (first_line) {
        std::string sbuf;
        getline (in, sbuf);
        if (sbuf.compare (0, strlen (first_line), first_line)) {
          in.close();
          throw Exception ("invalid first line for key/value file \"" + file + "\" (expected \"" + first_line + "\")");
        }
      }
      filename = file;
    }





    bool KeyValue::next ()
    {
      while (in.good()) {
        std::string sbuf;
        getline (in, sbuf);
        if (in.bad()) 
          throw Exception ("error reading key/value file \"" + filename + "\": " + strerror (errno));

        sbuf = strip (sbuf.substr (0, sbuf.find_first_of ('#')));
        if (sbuf == "END") {
          in.setstate (std::ios::eofbit);
          return false;
        }

        if (sbuf.size()) {
          size_t colon = sbuf.find_first_of (':');
          if (colon == std::string::npos) {
            INFO ("malformed key/value entry (\"" + sbuf + "\") in file \"" + filename + "\" - ignored");
          }
          else {
            K = strip (sbuf.substr (0, colon));
            V = strip (sbuf.substr (colon+1));
            if (K.empty()) {
              INFO ("malformed key/value entry (\"" + sbuf + "\") in file \"" + filename + "\" - ignored");
            }
            else 
              return true;
          }

        }
      }
      return false;
    }



  }
}


