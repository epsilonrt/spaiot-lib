/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SpaIot library is licensed under a
   Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.

   You should have received a copy of the license along with this
   work. If not, see <http://creativecommons.org/licenses/by-nc-sa/4.0/>.

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#pragma once

#include "global.h"

namespace SpaIot  {

  /**
     @class ServerSettings
     @brief Server settings class

      This class represents the settings of a SpaIot server.
      It is a concrete class, and can be instantiated.
      This class is copyable and movable.
  */
  class ServerSettings {

    public:
      /**
         The constructor
         takes a single parameter, which is the initial value of the
         ServerSettings object
      */
      ServerSettings ();

      ServerSettings (const String &model, unsigned long interval = 30000);
      ServerSettings (const char * model, unsigned long interval = 30000);

      /**
        @brief Destructor
      */
      virtual ~ServerSettings();

      /**
        @brief Copy Constructor
      */
      ServerSettings (const ServerSettings &other);

      /**
         @brief Move Constructor
         after the move, @b other is null, other.clear() **must be called** if it is to be used again
      */
      ServerSettings (ServerSettings &&other);

      /**
         @brief Sets the ServerSettings object to be equal to @b other.
      */
      ServerSettings &operator= (const ServerSettings &other);

      /**
         @brief Move assignment
          after the move, @b other is null, other.clear() **must be called** if it is to be used again
      */
      ServerSettings &operator= (ServerSettings &&other);

      /**
       * @brief Clears the ServerSettings object
       * After the call to this function, the ServerSettings object is the same as one that has been default constructed 
       */
      void clear();

      /**
         @brief Returns true if the object is null (i.e. it has not been initialized, d_ptr is null)
      */
      bool isNull() const;

      /**
         @brief Checks if all attributes are set to their default values (same as the default constructor)
      */
      bool isEmpty() const;

      /**
         The function returns true if the two objects are equal

         @param other the other ServerSettings object to compare to

         @return The return value is a boolean value.  It is true if the two objects are
         equal, and false if they are not equal.
      */
      bool operator== (const ServerSettings &other) const;

      /**
         The function returns true if the two objects are not equal

         @param other the other ServerSettings object to compare to

         @return The return value is a boolean value.  It is true if the two objects are
         not equal, and false if they are equal.
      */
      bool operator!= (const ServerSettings &other) const;

      /**
         @brief Returns the spa model name
      */
      const String &  spaModel() const;

      /**
         @brief Returns the publish interval
      */
      unsigned long   publishInterval() const;

      /**
         @brief Sets the spa model name
      */
      void           setPublishInterval (unsigned long interval);

      /**
         @brief Sets the publish interval
      */
      void          setSpaModel (const String &model);

    protected:
      class Private;
      ServerSettings (Private &dd);
      std::unique_ptr<Private> d_ptr;
    private:
      PIMPL_DECLARE_PRIVATE (ServerSettings)
  };

}
