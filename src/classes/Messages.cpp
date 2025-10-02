#include <Message.hpp>

Message::Message(int fd) : sender_(fd) {}
Message::Message(int fd, std::vector<std::string> content) : sender_(fd), content(content) {
	(void)sender_;
}
