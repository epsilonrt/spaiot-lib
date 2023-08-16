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

#include <set>
#include <initializer_list>
#include "event.h"

namespace SpaIot {

  /**
     @brief Client class

     This class is used to interface the spa with the external world (MQTT, HTTP, Alexa, ...).
     It's a base class that must be derived to implement the protocol with an external library (PubSubClient, SinricPro...).

     - When an event occurs at the spa level, it is sent to all connected clients using \c write(), this event is stored in a FIFO queue,
     - the client retrieves the event using \c pullFromSpa() and transmits it outside the spa (MQTT, HTTP, Alexa, ...).
     - When the user sends a command from outside the spa (MQTT, HTTP, Alexa, ...), the client transmits the command to the spa using \c pushToSpa(), this event is stored in a FIFO queue,
     - the spa retrieves the event using \c read() and executes it.
     .

     Here is a schematic representation of the communication between the spa and the clients:
     @verbatim
     +----------+               FiFo                     +-----------+
     |          | write() --> OOOOOOOO --> pullFromSpa() |           |
     |  Server  |                                        | Protected | <---> External world (MQTT, HTTP, Alexa, ...)
     |  (SPA)   | read()  <-- OOOOOOOO <---- pushToSpa() |    API    |       implements the protocol with an external library
     +----------+               FiFo                     +-----------+
                [ --------------   SpaIot::Client   --------------   ] [ --- External library (PubSubClient, SinricPro...) ---]
     @endverbatim
  */
  class Client {
    public:

      /**
         @brief Construct a new default Client object

         The subscribed events list is empty, all events may be sent to the client or the spa
      */
      Client();

      /**
         @brief Destructor
      */
      virtual ~Client();

      /**
         @brief Construct a new Client object with the specified subscribed events

         @param subscribedEvents The events subscribed by the client, only the events in the list will be sent to the client or the spa
      */
      Client (std::initializer_list<Event::Type> subscribedEvents);

      /**
         @brief Construct a new Client object with the specified subscribed events

         @param subscribedEvents  The events subscribed by the client, only the events in the list will be sent to the client or the spa
      */
      Client (std::set<Event::Type> subscribedEvents);

      /**
        @brief Copy Constructor
      */
      Client (const Client &other);

      /**
         @brief Move Constructor
         after the move, @b other is null, other.clear() **must be called** if it is to be used again
      */
      Client (Client &&other);

      /**
         @brief Sets the Client object to be equal to @b other.
      */
      Client &operator= (const Client &other);

      /**
         @brief Move assignment
          after the move, @b other is null, other.clear() **must be called** if it is to be used again
      */
      Client &operator= (Client &&other);

      /**
         @brief Clears the Client object
         After the call to this function, the Client object is the same as one that has been default constructed
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

         @param other the other Client object to compare to

         @return The return value is a boolean value.  It is true if the two objects are
         equal, and false if they are not equal.
      */
      bool operator== (const Client &other) const;

      /**
         The function returns true if the two objects are not equal

         @param other the other Client object to compare to

         @return The return value is a boolean value.  It is true if the two objects are
         not equal, and false if they are equal.
      */
      bool operator!= (const Client &other) const;

      /**
         @brief Add subscribed events

         @param subscribedEvents  The events subscribed by the client
      */
      void subscribe (std::initializer_list<Event::Type> subscribedEvents);

      /**
         @brief Add subscribed events

         @param subscribedEvents  The events subscribed by the client
      */
      void subscribe (std::set<Event::Type> subscribedEvents);

      /**
         @brief Add subscribed event

         @param event  The event subscribed by the client
      */
      void subscribe (Event::Type event);

      /**
         @brief Get the subscribed events

         @return subscribedEvents  The events subscribed by the client
      */
      const std::set<Event::Type> &subscribedEvents() const;

      /**
         @brief Check if the client is subscribed to the event

         @param event The event
         @return true if the client is subscribed to the event, false otherwise
      */
      bool isSubscribed (const Event::Type &event) const;

      /**
         @brief Write an event to the client

         @param event The event to be sent
      */
      bool write (const Event &event);

      /**
         @brief Reads the next event from the client

         @return event  The event, type is Event::NoEvent if the queue is empty
      */
      Event read ();

      /**
         @brief Reads the next event of the specified type from the client

         @param type  The event type that is expected
         @return Event  The event, type is Event::NoEvent if the queue is empty or the event type is not the expected one
      */
      Event read (Event::Type type);

      /**
         @brief Returns the number of events to be read
      */
      int available() const;

    protected:
      /** @name Client Protected API

          Used by client-derived classes to communicate the outside world to the spa
          @{
      */
      /**
         @brief This client push an event to the spa

         @param event  The event to be pushed
      */
      void pushToSpa (const Event &event);

      /**
         @brief This client pull an event from the spa

         @param event  The event to be filled
         @return true  if the event has been filled, false otherwise
      */
      bool pullFromSpa (Event &event);
      /**@}*/

    protected:
      class Private;
      Client (Private &dd);
      std::unique_ptr<Private> d_ptr;
    private:
      PIMPL_DECLARE_PRIVATE (Client)
  };
}