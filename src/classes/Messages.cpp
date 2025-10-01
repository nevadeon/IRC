#include <Message.hpp>

Message::Message(int fd) : sender_(fd) {}
<<<<<<< HEAD
Message::Message(int fd, std::vector<std::string> content) : sender_(fd), content(content) {
	(void)sender_;
}
=======
Message::Message(int fd, std::vector<std::string> content) : sender_(fd), content(content) {}
>>>>>>> 64321f4 (split commandes message)
