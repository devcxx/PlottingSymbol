#include "CommandManager.h"

CommandManager *CommandManager::instance()
{
    static CommandManager ins;
    return &ins;
}

void CommandManager::undo()
{
    Command * command = popUndoCommand();
    if (command) {
        if (command->unexecute()) {
            pushRedoCommand(command);
        } else {
            delete command;
        }
    }
}

void CommandManager::redo()
{
    Command * command = popRedoCommand();
    if (command) {
        if (command->execute()) {
            pushUndoCommand(command);
        } else {
            delete command;
        }
    }
}

void CommandManager::callCommand(Command *command)
{
    if (command != NULL) {
        if (command->execute()) {
            pushUndoCommand(command);
//            clearRedoCommand();
        } else {
            delete command;
        }
    }
}

void CommandManager::pushUndoCommand(Command *command)
{
    if (command != NULL) {
        m_stackUndo.push(command);
    }
}

void CommandManager::pushRedoCommand(Command *command)
{
    if (command != NULL) {
        m_stackRedo.push(command);
    }
}

void CommandManager::clearRedoCommand()
{
    while (!m_stackRedo.empty()) {
        delete m_stackRedo.top();
        m_stackRedo.pop();
    }
}

void CommandManager::clearUndoCommand()
{
    while (!m_stackUndo.empty()) {
        delete m_stackUndo.top();
        m_stackUndo.pop();
    }
}

Command *CommandManager::popUndoCommand()
{
    Command * command = NULL;
    if (!m_stackUndo.empty()) {
        command = m_stackUndo.top();
        m_stackUndo.pop();
    }
    return command;
}

Command *CommandManager::popRedoCommand()
{
    Command * command = NULL;
    if (!m_stackRedo.empty()) {
        command = m_stackRedo.top();
        m_stackRedo.pop();
    }
    return command;
}
