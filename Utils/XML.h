//https://github.com/Masafi/XMLParcer

#pragma once
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <iostream>

namespace XML {

	struct Node {
		std::vector<Node*> children;
		std::string name;
		std::string value;
		std::map<std::string, std::string> attributes;
		int depth;

		~Node() {
			for (auto i : children) {
				delete(i);
			}
		}

		friend std::ostream &operator<<(std::ostream &output, const Node &node) {
			std::string spaces(node.depth, ' ');
			output << spaces << "<" << node.name;
			for (auto i : node.attributes) {
				output << " " << i.first << "=\"" << i.second << "\"";
			}
			output << ">" << std::endl;
			if (!node.value.empty()) {
				output << spaces << " " << node.value << std::endl;
			}
			for (auto i : node.children) {
				output << (*i);
			}
			output << spaces << "</" << node.name << ">" << std::endl;
			return output;
		}

		std::string attr(const std::string& name, std::string dflt = "") const {
			if (attributes.find(name) == attributes.end()) {
				return dflt;
			}
			return attributes.at(name);
		}

		double attrd(const std::string& name, double dflt = 0) const {
			if (attributes.find(name) == attributes.end()) {
				return dflt;
			}
			return std::stod(attributes.at(name));
		}

		int attri(const std::string& name, int dflt = 0) const {
			if (attributes.find(name) == attributes.end()) {
				return dflt;
			}
			return std::stod(attributes.at(name));
		}
	};

	class File {
		std::string path;

		bool parce() {
			if (path.empty() || path == " " || path == "") {
				std::cerr << "ERROR [XML]: File path is empty" << std::endl;
				return 0;
			}
			std::ifstream file(path);
			if (!file.good() || !file.is_open()) {
				std::cerr << "ERROR [XML]: Cannot open XML file: " << path << std::endl;
				return 0;
			}
			if (file.eof()) {
				std::cerr << "WARNING [XML]: File is empty" << std::endl;
			}

			enum class Condition {
				Reading,
				Comment,
				InTag,
				InCloseTag,
				AttrName,
				AttrValue
			};

			Condition con = Condition::Reading;
			Condition prev = Condition::Reading;
			std::vector<Node*> stack;
			root = new Node();
			root->name = path;
			root->depth = -1;
			stack.push_back(root);
			std::string temp;
			std::string stemp;
			bool good = 1;
			bool tempb = 0;
			while (!file.eof()) {
				std::string input;
				file >> input;
				input += " ";
				for (int i = 0; i < input.size(); i++) {
					if (con != Condition::Comment && input.size() - i >= 4 && input.compare(i, 4, "<!--") == 0) {
						prev = con;
						con = Condition::Comment;
						continue;
					}
					switch (con) {
					case Condition::Reading: {
						switch (input[i]) {
						case '<':
							stack.back()->value += temp;
							temp = "";
							if (input[i + 1] == '/') {
								con = Condition::InCloseTag;
								i++;
							}
							else {
								con = Condition::InTag;
							}
							break;
						case '>':
							std::cerr << "WARNING [XML]: Nothing to close" << std::endl;
							good = 0;
							break;
						default:
							temp += input[i];
							break;
						}
						break;
					}
					case Condition::Comment: {
						if (input.size() - i >= 3 && input.compare(i, 3, "-->") == 0) {
							con = prev;
							i += 3;
						}
						if (input[i + 1] == ' ') {
							i++;
						}
						break;
					}
					case Condition::InTag: {
						if (input[i] == ' ' || input[i] == '>') {
							Node* node = new Node();
							node->name = temp;
							node->depth = stack.back()->depth + 1;
							temp = "";
							stack.back()->children.push_back(node);
							stack.push_back(node);
							if (input[i] == ' ') {
								con = Condition::AttrName;
							}
							else {
								con = Condition::Reading;
								if (i != 0 && input[i - 1] == '/') {
									stack.pop_back();
								}
								if (input[i + 1] == ' ') {
									i++;
								}
							}
						}
						else {
							temp += input[i];
						}
						break;
					}
					case Condition::InCloseTag: {
						if (input[i] == '>') {
							if (stack.back()->name != temp) {
								std::cerr << "WARNING [XML]: Close tag name isn't equal to start name" << std::endl;
								good = 0;
							}
							if (stack.size() <= 1) {
								std::cerr << "ERROR [XML]: Closing root tag (nothing to close)" << std::endl;
								return 0;
							}
							temp = "";
							stack.pop_back();
							con = Condition::Reading;
							if (input[i + 1] == ' ') {
								i++;
							}
						}
						else {
							temp += input[i];
						}
						break;
					}
					case Condition::AttrName: {
						switch (input[i]) {
						case '>':
							if (!temp.empty()) {
								std::cerr << "WARNING [XML]: Tag closed, but attr not finished (" << temp << ")" << std::endl;
								good = 0;
								temp = "";
							}
							if (i != 0 && input[i - 1] == '/') {
								if (stack.size() <= 1) {
									std::cerr << "ERROR [XML]: Closing root tag (nothing to close)" << std::endl;
									return 0;
								}
								stack.pop_back();
							}
							con = Condition::Reading;
							if (input[i + 1] == ' ') {
								i++;
							}
							break;
						case ' ':
						case '=':
							con = Condition::AttrValue;
							tempb = 0;
							break;
						case '/':
							break;
						default:
							temp += input[i];
							break;
						}
						break;
					}
					case Condition::AttrValue: {
						switch (input[i]) {
						case '\"':
							if (!tempb) {
								tempb = 1;
							}
							else {
								stack.back()->attributes.insert({ temp, stemp });
								temp = "";
								stemp = "";
								con = Condition::AttrName;
								if (input[i + 1] == ' ') {
									i++;
								}
							}
							break;
						default:
							if (tempb) {
								stemp += input[i];
							}
							break;
						}
						break;
					}
					default: {
						std::cerr << "ERROR [XML]: Unknown condition" << std::endl;
						return 0;
						break;
					}
					}
				}
			}
			if (stack.size() != 1) {
				std::cerr << "ERROR [XML]: Finished not in the root node, last is: " << stack.back()->name << std::endl;
				good = 0;
			}
			if (con != Condition::Reading) {
				std::cerr << "ERROR [XML]: Finished with strange condition (not in the root node): " << (int)con << std::endl;
				good = 0;
			}
			return good;
		}
	public:
		Node* root;

		File() {
			root = nullptr;
		}
		~File() {
			delete(root);
		}

		void setPath(const std::string& path) {
			this->path = path;
		}
		bool open() {
			bool v = parce();
			if (root) {
				root->depth = 0;
			}
			return v;
		}
		bool open(const char path[]) {
			setPath(std::string(path));
			return open();
		}
		bool open(const std::string& path) {
			setPath(path);
			return open();
		}

		void save() {
			save(path);
		}
		void save(const char path[]) {
			save(std::string(path));
		}
		void save(const std::string& path) {
			setPath(path);
			std::ofstream file(path);
			save(file);
		}
		void save(std::ostream &stream) {
			for (auto i : root->children) {
				stream << (*i);
			}
		}
	};
}