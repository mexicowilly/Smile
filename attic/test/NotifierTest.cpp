#include "NotifierTest.hpp"
#include <smile/Notifier.hpp>

namespace
{

class InvalidNotificationException : public std::exception
{
public:
    virtual const char* what() const throw();
};

class Command
{
public:
    Command();

    void action(const smile::Notification& notification);
    const smile::Notification& getNotification();
    bool hasNotification() const;
    void reset();

private:
    const smile::Notification* m_notification;
};

const char* InvalidNotificationException::what() const throw()
{
    return "The notification was not initialized";
}

Command::Command()
    : m_notification(0)
{
}

void Command::action(const smile::Notification& notification)
{
    m_notification = &notification;
}

const smile::Notification& Command::getNotification()
{
    if (m_notification == 0)
        throw InvalidNotificationException();
    return *m_notification;
}

bool Command::hasNotification() const
{
    return m_notification != 0;
}

void Command::reset()
{
    m_notification = 0;
}

}

void NotifierTest::testAnySender()
{
    Command command;
    smile::Notifier& notifier = smile::Notifier::getDefault();
    notifier.addObserver(command, &Command::action);
    smile::Notification note("hello", this);
    notifier.post(note);
    predicate(HERE, command.hasNotification());
    predicate(HERE, command.getNotification() == note);
    command.reset();
    Command command2;
    notifier.addObserver(command2, &Command::action);
    note = smile::Notification("chunky", &notifier);
    notifier.post(note);
    predicate(HERE, command.hasNotification());
    predicate(HERE, command.getNotification() == note);
    predicate(HERE, command2.hasNotification());
    predicate(HERE, command2.getNotification() == note);
    notifier.removeObserver(command);
    notifier.removeObserver(command2);
}

void NotifierTest::testData()
{
    smile::Notifier& notifier = smile::Notifier::getDefault();
    Command command;
    notifier.addObserver(command, &Command::action);
    smile::Properties props;
    props.setBool("what", true);
    smile::Notification note("hello", this, props);
    notifier.post(note);
    predicate(HERE, command.hasNotification());
    predicate(HERE, command.getNotification() == note);
    notifier.removeObserver(command);
}

void NotifierTest::testName()
{
    smile::Notifier& notifier = smile::Notifier::getDefault();
    Command command;
    notifier.addObserver(command, &Command::action, smile::Text("hello"));
    smile::Notification note("hello", this);
    notifier.post(note);
    predicate(HERE, command.hasNotification());
    predicate(HERE, command.getNotification() == note);
    command.reset();
    note = smile::Notification("goodbye", this);
    notifier.post(note);
    predicate(HERE, !command.hasNotification());
    command.reset();
    note = smile::Notification("adios", &notifier);
    notifier.post(note);
    predicate(HERE, !command.hasNotification());
    command.reset();
    note = smile::Notification("hello", &notifier);
    notifier.post(note);
    predicate(HERE, command.hasNotification());
    predicate(HERE, command.getNotification() == note);
    notifier.removeObserver(command);
}

void NotifierTest::testNameAndSender()
{
    smile::Notifier& notifier = smile::Notifier::getDefault();
    Command command;
    notifier.addObserver(command, &Command::action, smile::Text("hello"), this);
    smile::Notification note("hello", this);
    notifier.post(note);
    predicate(HERE, command.hasNotification());
    predicate(HERE, command.getNotification() == note);
    command.reset();
    note = smile::Notification("hello", &notifier);
    notifier.post(note);
    predicate(HERE, !command.hasNotification());
    command.reset();
    note = smile::Notification("goodbye", this);
    notifier.post(note);
    predicate(HERE, !command.hasNotification());
    command.reset();
    notifier.removeObserver(command);
}

void NotifierTest::testSender()
{
    smile::Notifier& notifier = smile::Notifier::getDefault();
    Command command;
    notifier.addObserver(command, &Command::action, this);
    smile::Notification note("hello", this);
    notifier.post(note);
    predicate(HERE, command.hasNotification());
    predicate(HERE, command.getNotification() == note);
    command.reset();
    note = smile::Notification("creepy", &notifier);
    notifier.post(note);
    predicate(HERE, !command.hasNotification());
    notifier.removeObserver(command);
}
