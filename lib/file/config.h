#ifndef __file_config_h__
#define __file_config_h__

#include <map>
#include "file/key_value.h"

namespace MR
{
  namespace File
  {
    class Config
    {
      public:

        static void init ();

        static void set (const std::string& key, const std::string& value) {
          config[key] = value;
        }
        static std::string get (const std::string& key) {
          std::map<std::string, std::string>::iterator i = config.find (key);
          return (i != config.end() ? i->second : "");
        }
        static std::string get (const std::string& key, const std::string& default_value) {
          std::map<std::string, std::string>::iterator i = config.find (key);
          return (i != config.end() ? i->second : default_value);
        }

        static void set_bool (const std::string& key, bool value) {
          set (key, (value ? "true" : "false"));
        }
        static bool get_bool (const std::string& key, bool default_value);

        static void set_int (const std::string& key, int value) {
          set (key, str (value));
        }
        static int get_int (const std::string& key, int default_value);

        static void set_float (const std::string& key, float value) {
          set (key, str (value));
        }
        static float get_float (const std::string& key, float default_value);

      private:
        static std::map<std::string, std::string> config;
    };
  }
}

#endif

