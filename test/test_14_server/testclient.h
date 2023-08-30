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

#include <client.h>

class TestClient : public SpaIot::Client {
  public:
    TestClient ();
    TestClient (std::initializer_list <SpaIot::Event::Type> subscribedEvents);
    TestClient (const std::set <SpaIot::Event::Type> &subscribedEvents);

    bool begin();
    /**
     * @brief Process the tests
     * 
     * @return true while test is running
     */
    bool handle();

    protected:
      class Private;
      TestClient (Private &dd);
    private:
      PIMPL_DECLARE_PRIVATE (TestClient)
};
