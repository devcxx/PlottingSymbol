#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <stack>

/**
 * Undo和Redo框架
 */\

struct Command {
    virtual bool execute() = 0;
    virtual bool unexecute() = 0;
};

class CommandManager {
public:
    static CommandManager* instance();
    void undo();
    void redo();
    void callCommand(Command* command);

protected:
    void pushUndoCommand(Command* command);
    void pushRedoCommand(Command* command);
    void clearRedoCommand();
    void clearUndoCommand();

    Command * popUndoCommand();
    Command* popRedoCommand();

private:
    std::stack<Command *> m_stackUndo;
    std::stack<Command *> m_stackRedo;

private:
    CommandManager(){};
    CommandManager(const CommandManager&);
    CommandManager& operator=(const CommandManager&);
};

#endif
