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
     @brief SpaClient class

     This class is used to interface the spa with the external world (MQTT, HTTP, Alexa, ...).
     It's a base class that must be derived to implement the protocol with an external library (PubSubClient, SinricPro...).

     - When an event occurs at the spa level, it is sent to all connected clients using \c write(), this event is stored in a FIFO queue,
     - the client retrieves the event using \c pullFromSpa() and transmits it outside the spa (MQTT, HTTP, Alexa, ...).
     - When the user sends a command from outside the spa (MQTT, HTTP, Alexa, ...), the client transmits the command to the spa using \c pushToSpa(), this event is stored in a FIFO queue,
     - the spa retrieves the event using \c read() and executes it.
     .

     Here is a schematic representation of the communication between the spa and the clients:
     @verbatim
     +-----------+               FiFo                     +-----------+
     |           | write() --> OOOOOOOO --> pullFromSpa() |           |
     | SpaServer |                                        | Protected | <---> External world (MQTT, HTTP, Alexa, ...)
     |   (SPA)   | read()  <-- OOOOOOOO <---- pushToSpa() |    API    |       implements the protocol with an external library
     +-----------+               FiFo                     +-----------+
                 [ ----------------   SpaClient   ----------------   ] [ --- External library (PubSubClient, SinricPro...) ---]
     @endverbatim

     The communication between server and clients is asynchronous, clients can be connected or disconnected at any time.
     When the spa receives a command from a client, it executes it, then if the command has modified the state of the spa, it sends an event to all connected clients.
     @warning Thus a client must not block its process to wait for an event from the spa or the system will be blocked.
  */
  class SpaClient {
    public:

      /**
         @brief Construct a new default SpaClient object

         The subscribed events list is empty, all events may be sent to the client or the spa
      */
      SpaClient();

      /**
         @brief Destructor
      */
      virtual ~SpaClient();

      /**
         @brief Construct a new SpaClient object with the specified subscribed events

         @param subscribedEvents The events subscribed by the client, only the events in the list will be sent to the client or the spa
      */
      SpaClient (std::initializer_list<Event::Type> subscribedEvents);

      /**
         @brief Construct a new SpaClient object with the specified subscribed events

         @param subscribedEvents  The events subscribed by the client, only the events in the list will be sent to the client or the spa
      */
      SpaClient (const std::set<Event::Type> &subscribedEvents);

      /**
        @brief Copy Constructor
      */
      SpaClient (const SpaClient &other);

      /**
         @brief Move Constructor
         after the move, @b other is null, other.clear() **must be called** if it is to be used again
      */
      SpaClient (SpaClient &&other);

      /**
         @brief Sets the SpaClient object to be equal to @b other.
      */
      SpaClient &operator= (const SpaClient &other);

      /**
         @brief Move assignment
          after the move, @b other is null, other.clear() **must be called** if it is to be used again
      */
      SpaClient &operator= (SpaClient &&other);

      /**
         @brief Clears the SpaClient object
         After the call to this function, the SpaClient object is the same as one that has been default constructed
      */
      virtual void clear();

      /**
         @brief Returns true if the object is null (i.e. it has not been initialized, d_ptr is null)
      */
      bool isNull() const;

      /**
         @brief Checks if all attributes are set to their default values (same as the default constructor)
      */
      virtual bool isEmpty() const;

      /**
         The function returns true if the two objects are equal

         @param other the other SpaClient object to compare to

         @return The return value is a boolean value.  It is true if the two objects are
         equal, and false if they are not equal.
      */
      virtual bool operator== (const SpaClient &other) const;

      /**
         The function returns true if the two objects are not equal

         @param other the other SpaClient object to compare to

         @return The return value is a boolean value.  It is true if the two objects are
         not equal, and false if they are equal.
      */
      virtual bool operator!= (const SpaClient &other) const;

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

      /**
         @brief Start the client

         This method must be called in the setup() function before begin().
         It will be call by \c SpaServer::begin() to start the client services.
         The default implementation sets isOpen() to true and returns true.
         It must be reimplemented in derived classes to start the client services, and \b called by the derived class begin() method.
         @return true if the client has been started, false otherwise
      */
      bool begin();

      /**
         @brief Check if the client is open

         @return true if begin() has been called and the client is open, false otherwise
      */
      virtual bool isOpen() const;

      /**
         @brief Stop the client

         This method closes the client and stops the client services.
         It may be reimplemented in derived classes to stop the client services, and \b called by the derived class end() method.
         The default implementation sets isOpen() to false.
      */
      virtual void end();

      /**
         @brief Process the client

         This method must be called periodically in the main loop.
         It will be call by \c SpaServer::handle() to process the client tasks.
         The default implementation call \c pollSpaEvents() and returns true if events have been polled.
         It should be reimplemented in derived classes to process the client tasks.

         @return true if events have been processed from spa, false otherwise
      */
      virtual bool handle();

      /**
         @brief Returns the class name
         This allows to know the class name of the object at runtime when only a pointer to the base class is available
         It should be note that typeid().name() is not available on ESP32/ESP8266 because RTTI is not enabled.
      */
      virtual const String &className() const;

    protected:
      /** @name SpaClient Protected API

          Used by client-derived classes to communicate the outside world to the spa
          @{
      */
      
      /**
         @brief Construct a new SpaClient object with the specified class name
      */
      SpaClient (const char *className);

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

      /**
         @brief This client poll events from the spa

         Poll events from the spa using \c pullFromSpa() and update the internal variables of the client object.
         These variables are accessible by the \c spaValue() method.

         @return true  if events have been polled, false otherwise
      */
      virtual bool pollSpaEvents ();

      /**
         @brief Get a spa value from the internal variables of the client object

         These variables are updated by the \c pollSpaEvents() method.

         @param type   The event type
         @return uint16_t   The spa value, UnsetValue16 or UnsetValue8 (for boolean) if no value is available
      */
      uint16_t spaValue (Event::Type type) const;
      /**@}*/

    protected:
      class Private;
      SpaClient (Private &dd);
      std::unique_ptr<Private> d_ptr;
    private:
      PIMPL_DECLARE_PRIVATE (SpaClient)
  };
}