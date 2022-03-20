/*
 * SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
 * This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>
 *
 * SpaIot library is licensed under a
 * Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
 *
 * You should have received a copy of the license along with this
 * work. If not, see <http://creativecommons.org/licenses/by-nc-sa/4.0/>.
 *
 * SpaIot library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY;
 */
#pragma once

#include <map>
#include "global.h"

namespace SpaIot {

  /**
   * @class ButtonController
   * @brief
   */
  class ButtonController {
    public:
      /**
       * @brief
       */
      ButtonController();
      /**
       * @brief 
       * @param name
       */
      ButtonController (const String & name);
      /**
       * @brief
       * @return
       */
      virtual ~ButtonController();
      /**
       * @brief
       */
      virtual void begin() = 0;
      /**
       * @brief
       */
      virtual void end();
      /**
       * @brief
       * @param button
       * @return
       */
      virtual int select (int button) = 0;
      /**
       * @brief
       */
      virtual void deselect () = 0;
      /**
       * @brief
       * @return
       */
      virtual int selected() const;
      /**
       * @brief
       * @return
       */
      virtual bool isOpened() const;
      /**
       * @brief
       * @return
       */
      virtual bool isNull() const;
      /**
       * @brief
       * @return
       */
      virtual bool isSelected() const;
      /**
       * @brief 
       * @return 
       */
      virtual const String & name() const;
      /**
       * @brief
       * @param other
       */
      virtual bool operator== (const ButtonController &other) const;
      /**
       * @brief
       * @param other
       */
      virtual bool operator!= (const ButtonController &other) const;
      /**
       * @brief
       * @param name
       * @param controller
       * @return
       */
      static bool addToRegister (const String & name, ButtonController & controller);
      /**
       * @brief
       * @param name
       * @return
       */
      static ButtonController & getFromRegister (const String & name);
      /**
       * @brief 
       * @param name
       * @return 
       */
      static bool registerContains (const String & name);

    protected:
      bool m_isopened;
      int m_selected;
      mutable String m_name;

    private:
      static std::map<String, ButtonController &> Register;
  };
}
