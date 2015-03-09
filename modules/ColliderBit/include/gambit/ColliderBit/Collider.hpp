#pragma once
#include "gambit/ColliderBit/BaseCollider.hpp"

namespace Gambit {
  namespace ColliderBit {

/// @note To configure a new collider, follow these steps:
/// @note STEP1)  BOSS / Backend your favorite collider simulator.
/// @note STEP2)  Sub-class BaseCollider. The type of event is the template parameter.

    class TemplatePythia : public BaseCollider<Pythia8::Event> {
      protected:
      /// @name Member variables and custom exceptions
      //@{
        Pythia8::Pythia* _pythiaInstance;
        std::vector<std::string> _settings;
        class InitializationError : public std::exception {
          virtual const char* what() const throw() {
            return "For whatever reason, Pythia could not initialize.";
          }
        };
        class EventFailureError : public std::exception {
          virtual const char* what() const throw() {
            return "For whatever reason, Pythia could not make the next event.";
          }
        };
      //@}

      public:
        TemplatePythia() : _pythiaInstance(nullptr) {
          _settings.push_back("../extras/boss/bossed_pythia_source/xmldoc/");
        }
        ~TemplatePythia() { delete _pythiaInstance; }

      /// @name Member variable access
      //@{
        std::vector<std::string> const getSettings() const { return _settings; }
        Pythia8::Pythia* const pythia() const { return _pythiaInstance; }
      //@}

      /// @name (Re-)Initialization functions
      //@{
        /// @brief Settings parsing and initialization for each sub-class
        virtual void init(const std::vector<std::string>& settings);
        /// @brief Special, hard-coded init. Template specializations go in cpp file.
        template <typename Tag>
        void specialize() { throw UnknownTagError(); }

        /// @brief Prepare for a new Pythia configuration
        void reset() {
          _settings.clear(); _settings.push_back("../extras/boss/bossed_pythia_source/xmldoc/");
          delete _pythiaInstance; _pythiaInstance=nullptr;
        }
        /// @brief Send a command to the Pythia instance
        void set(const std::string& command) { _pythiaInstance->readString(command); }
      //@}

      /// @name Event generation functions - Required to be const by Gambit
      //@{
        /// @brief Fill in the next collider event by reference according to a tag.
        template <typename Tag>
        void nextEventByTag(EventType& event) const { throw UnknownTagError(); }
        /// @brief Fill in the next collider event by reference without a tag.
        virtual void nextEvent(EventType& event) const;
      //@}

    };

    /// @brief Recycle to a new Pythia initialization via name (Template parameter).
    bool recycleTemplatePythia(TemplatePythia&, const std::string&,
                               const std::vector<std::string>&);

/// @note STEP3-5)  Continue to colliders/Collide_Pythia.cpp
  }
}
