

#include <cstring>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

std::vector<std::string> &split(const std::string &s, char delim,
		std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	elems.erase(std::remove(elems.begin(), elems.end(), ""), elems.end());
	return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

char buffer[1024];

char *my_dirname(const char *pathname) {
	if (!pathname) {
		strcpy(buffer, ".");
		return buffer;
	}
	std::string pn(pathname);
	pn.erase(std::remove(pn.begin(), pn.end(), ' '), pn.end());
	std::vector<std::string> v = split(pn, '/');
	if (v.size() == 0) {
		if (pn.find('/') != std::string::npos) {
			strcpy(buffer, "/");
		} else {
			strcpy(buffer, ".");
		}
	} else {
		std::string ret("");
		for (std::vector<std::string>::iterator it = v.begin(); it != v.end() - 1;
				it++) {
			ret += "/";
			ret += *it;
		}
		if(ret == "")
			ret = ".";
		strcpy(buffer, ret.c_str());
	}
	return buffer;
}

char *my_basename(const char *pathname) {
	if (!pathname) {
		strcpy(buffer, ".");
		return buffer;
	}
	std::string pn(pathname);
	pn.erase(std::remove(pn.begin(), pn.end(), ' '), pn.end());
	std::vector<std::string> v = split(pn, '/');
	if (v.size() == 0) {
		if (pn.find('/') != std::string::npos) {
			strcpy(buffer, "/");
		} else {
			strcpy(buffer, ".");
		}
	} else {
		strcpy(buffer, (v.end() - 1)->c_str());
	}
	return buffer;
}



int test() {

	/*
	 / 					/ 			/
	 /usr/bin/zip	 	/usr/bin 	zip
	 /etc/passwd////	/etc 		passwd
	 /etc////passwd 	/etc 		passwd
	 etc/passwd		 	etc 		passwd
	 passwd 			. 			passwd
	 passwd/ 			. 			passwd
	 .. 				. 			..
	 NULL 				.			 .
	 */

	char * ret;
	ret = my_basename("/ ");
	std::cout << ret << std::endl;
	ret = my_dirname("/ ");
	std::cout << ret << std::endl << std::endl;

	ret = my_basename(" /usr/bin/zip");
	std::cout << ret << std::endl;
	ret = my_dirname(" /usr/bin/zip");
	std::cout << ret << std::endl << std::endl;

	ret = my_basename("/etc/passwd////");
	std::cout << ret << std::endl;
	ret = my_dirname("/etc/passwd////");
	std::cout << ret << std::endl << std::endl;

	ret = my_basename(" /etc////passwd");
	std::cout << ret << std::endl;
	ret = my_dirname(" /etc////passwd");
	std::cout << ret << std::endl << std::endl;

	ret = my_basename("passwd");
	std::cout << ret << std::endl;
	ret = my_dirname("passwd");
	std::cout << ret << std::endl << std::endl;

	ret = my_basename(" passwd/");
	std::cout << ret << std::endl;
	ret = my_dirname(" passwd/");
	std::cout << ret << std::endl << std::endl;

	ret = my_basename(".. ");
	std::cout << ret << std::endl;
	ret = my_dirname("..");
	std::cout << ret << std::endl << std::endl;

	ret = my_basename(0);
	std::cout << ret << std::endl;
	ret = my_dirname(0);
	std::cout << ret << std::endl << std::endl;

	return 0;
}





