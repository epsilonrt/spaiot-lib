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
  
  class ButtonController;
  
  /**
   * @class ButtonSettings
   * @brief
   */
  class ButtonSettings {
    public:
      /**
       * @brief 
       */
      ButtonSettings();
      /**
       * @brief 
       * @param controllerName
       * @param buttonId
       */
      ButtonSettings (const String & controllerName, int buttonId);
      /**
       * @brief 
       * @param controller
       * @param buttonId
       */
      ButtonSettings (ButtonController & controller, int buttonId);
      /**
       * @brief 
       * @return 
       */
      const String & controllerName() const;
      /**
       * @brief 
       * @return 
       */
      int id() const;
      /**
       * @brief 
       * @param id
       */
      void setId (int id);
      /**
       * @brief 
       * @return 
       */
      bool isNull() const;
      /**
       * @brief 
       * @param other
       */
      bool operator== (const ButtonSettings &other) const;
      /**
       * @brief 
       * @param other
       */
      bool operator!= (const ButtonSettings &other) const;
      /**
       * @brief 
       * @return 
       */
      const ButtonController & ctrl() const;
      /**
       * @brief 
       * @return 
       */
      ButtonController & ctrl();

    private:
      int m_id;
      String  m_ctrlName;
      mutable ButtonController * m_ctrl;
  };

  /**
   * @brief 
   */
  extern const std::map<int, ButtonSettings> Scip2SspButtons;
  /**
   * @brief 
   */
  extern const std::map<int, ButtonSettings> Scip2SjbButtons;
}
