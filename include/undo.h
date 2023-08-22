#ifndef MINIDOC_UNDO_H
#define MINIDOC_UNDO_H

#include <deque>
#include <darcs_types.h>
#include <stdio.h>

namespace MiniDoc {

  template<typename T>
  class UndoStack {

    struct InvertableCommand {
      virtual std::shared_ptr<InvertableCommand> invert(std::shared_ptr<InvertableCommand> & cmd) const = 0;
      virtual const InvertableCommand * get_command() const {
        return this;
      }
      virtual const bool is_inverted() const {
        return false;
      }
      virtual ~InvertableCommand() = default;
      virtual std::ostream & to_stream(std::ostream & os, bool is_inverted) const {
        return os << "InvertableCommand";
      }

      friend std::ostream & operator<<(std::ostream & os, const InvertableCommand & cmd) {
          return cmd.to_stream(os, cmd.is_inverted());
      }
    };

    public:

    struct Command : public InvertableCommand {
      virtual void undo(T * instance) = 0;
      virtual void redo(T * instance) = 0;
      std::ostream & to_stream(std::ostream & os, bool is_inverted) const override {
        return os << "Command";
      }
      virtual ~Command() = default;
      const bool is_inverted() const override {
        return false;
      }
      const Command * get_command() const override {
        return this;
      }
      protected:
      private:
      std::shared_ptr<InvertableCommand> invert(std::shared_ptr<InvertableCommand> & cmd) const override {
        return std::shared_ptr<InvertableCommand>(new InvertCommand(cmd), [](auto * p) { delete p; });
      }
    };

    private:

    struct InvertCommand : public Command {
      std::shared_ptr<InvertableCommand> cmd;

      InvertCommand(std::shared_ptr<InvertableCommand> & cmd) : cmd(cmd) {}

      void undo(T * instance) override {
        static_cast<Command*>(cmd.get())->redo(instance);
      };
      void redo(T * instance) override {
        static_cast<Command*>(cmd.get())->undo(instance);
      }
      std::ostream & to_stream(std::ostream & os, bool is_inverted) const override {
        os << "InvertCommand: ";
        cmd->to_stream(os, is_inverted);
        return os;
      }
      const bool is_inverted() const override {
        return true;
      }
      const Command * get_command() const override {
        return static_cast<const Command*>(cmd.get());
      }
      private:
      std::shared_ptr<InvertableCommand> invert(std::shared_ptr<InvertableCommand> & cmd) const override {
        return this->cmd;
      }
    };

    using STACK = std::vector<std::shared_ptr<InvertableCommand>>;

    STACK undo_stack;
    STACK redo_stack;
    
    public:

    bool supports_redo = true;
    bool supports_advanced_undo = true;
    
    std::size_t undoSize() {
        return undo_stack.size();
    }
    
    std::size_t redoSize() {
        return redo_stack.size();
    }

    private:

    InvertableCommand & deref(std::shared_ptr<InvertableCommand> & cmd) const {
      InvertableCommand & ref = *cmd;
      return ref;
    }

    public:
    
    void push(Command * command) {
      // std::cout << "BEFORE PUSH: " << std::endl << *this << std::endl;
      auto cmd = std::shared_ptr<InvertableCommand>(command, [](auto p) { delete p; });
      if (supports_redo) {
        if (!supports_advanced_undo) {
          // std::cout << "erasing redo stack" << std::endl;
          redo_stack = {};
          // std::cout << "erased redo stack" << std::endl;
        } else {
          if (redo_stack.size() != 0) {
            // std::cout << "pushing redo stack to undo stack in reverse order" << std::endl;
            for (auto i = redo_stack.rbegin(); i != redo_stack.rend(); i++) {
              auto command2 = *i;
              // std::cout << "pushing command to undo stack: " << deref(command2) << std::endl;
              undo_stack.insert(undo_stack.end(), command2);
            }
            // std::cout << "pushed redo stack to undo stack in reverse order" << std::endl;
            // std::cout << "pushing inverted redo stack to undo stack" << std::endl;
            for (auto i = redo_stack.begin(); i != redo_stack.end(); i++) {
              auto command2 = i->get()->invert(*i);
              // std::cout << "pushing command to undo stack: " << deref(command2) << std::endl;
              undo_stack.insert(undo_stack.end(), command2);
            }
            // std::cout << "pushed inverted redo stack to undo stack" << std::endl;
            // std::cout << "erasing redo stack" << std::endl;
            redo_stack = {};
            // std::cout << "erased redo stack" << std::endl;
          }
        }
      }
      // std::cout << "pushing command to undo stack: " << deref(cmd) << std::endl;
      undo_stack.push_back(cmd);
      // std::cout << "AFTER PUSH: " << std::endl << *this << std::endl;
    }

    std::size_t undo_to_index(std::size_t index, T * instance) {
      std::size_t old = undo_stack.size();
      while (undo_stack.size() != index) {
        undo(instance);
      }
      return old;
    }

    void redo_to_index(std::size_t index, T * instance) {
      while (undo_stack.size() != index) {
        redo(instance);
      }
    }

    void transform_undo_stack(const DarcsPatch::function<void(Command*)> & f) {
      for (auto & cmd : undo_stack) {
        f(static_cast<Command*>(cmd.get()));
      }
    }

    void transform_redo_stack(const DarcsPatch::function<void(Command*)> & f) {
      for (auto & cmd : redo_stack) {
        f(static_cast<Command*>(cmd.get()));
      }
    }

    const Command * get_index(std::size_t index) const {
      if (undo_stack.size() == 0) {
        return nullptr;
      }
      if (index > undo_stack.size()-1) {
        return nullptr;
      }
      return static_cast<const Command*>(undo_stack[index].get());
    }
    
    bool undo(T * instance) {
      if (undo_stack.size() == 0) {
        return false;
      }
      auto cmd = undo_stack.back();
      if (supports_redo) {
        redo_stack.push_back(cmd);
      }
      // std::cout << "undo() : command: " << deref(cmd) << std::endl;
      static_cast<Command*>(cmd.get())->undo(instance);
      undo_stack.pop_back();
      return true;
    }
    
    bool redo(T * instance) {
      if (!supports_redo || redo_stack.size() == 0) {
        return false;
      }
      auto cmd = redo_stack.back();
      // std::cout << "redo() : command: " << deref(cmd) << std::endl;
      static_cast<Command*>(cmd.get())->redo(instance);
      undo_stack.push_back(cmd);
      redo_stack.pop_back();
      return true;
    }

    virtual ~UndoStack() {
    }
    
    void reset() {
      // puts("RESET REDO");
      std::cout << *this << std::endl;
      if (undo_stack.size() != 0) {
        undo_stack = std::move(STACK());
      }
      if (redo_stack.size() != 0) {
        redo_stack = std::move(STACK());
      }

    }

    virtual std::ostream & to_stream(std::ostream & os) const {
      os << "Undo Stack: " << std::to_string(undo_stack.size()) << " items in undo stack" << std::endl;
      if (undo_stack.size() != 0) {
        std::size_t idx = 0;
        for (auto & shared_command : undo_stack) {
          os << "    undo #" << std::to_string(idx) << " : ";
          shared_command->to_stream(os, shared_command->is_inverted()) << std::endl;
          idx++;
        }
      }
      os << "Undo Stack: " << std::to_string(redo_stack.size()) << " items in redo stack" << std::endl;
      if (redo_stack.size() != 0) {
        std::size_t idx = 0;
        for (auto & shared_command : redo_stack) {
          os << "    redo #" << std::to_string(idx) << " : ";
          shared_command->to_stream(os, shared_command->is_inverted()) << std::endl;
          idx++;
        }
      }
      return os;
    }
  };

  template <typename T>
  std::ostream & operator<<(std::ostream & os, const UndoStack<T> & stack) {
      return stack.to_stream(os);
  }
}
#endif