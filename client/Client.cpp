#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <string>
#include <ctype.h>
#include <cstring>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sstream>
#include <fstream>

using namespace std;

int sock;
struct sockaddr_in client;
int PORT = 80;

void loadingBar() {

    int width = 15;

    for(int i = 0; i <= 100; ++i) {
        int full = (double)i / 100 * width;

        cout << string(100, '\n');
        cout << setw(3) << i;
        cout << "%  [";

        for(int j = 0; j < full; ++j) {
            cout << "█";
        }
        cout << string(width - full, ' ');

        cout << "]" << string(8, '\n');
        cout.flush();
        usleep(50*100);
    }
}

string getSummary(string text) {

    struct hostent * host = gethostbyname("http://127.0.0.1:5000");

    if ((host == NULL) || (host->h_addr == NULL)) {
        cout << "Error in DNS info retrieval" << endl;
        exit(1);
    }

    bzero(&client, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = htons(PORT);
    memcpy(&client.sin_addr, host->h_addr, host->h_length);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        cout << "Error in socket creation" << endl;
        exit(1);
    }

    if (connect(sock, (struct sockaddr *)&client, sizeof(client)) < 0) {
        close(sock);
        cout << "Error - couldn't connect" << endl;
        exit(1);
    }

    stringstream ss;
    ss << "GET /sum/" << text << " HTTP/1.0\r\n"
    << "Host: http://127.0.0.1:5000"
    << "Accept: application/json\r\n"
    << "\r\n\r\n";

    string request = ss.str();

    if (send(sock, request.c_str(), request.length(), 0) != (int)request.length()) {
        cout << "Error in sending request" << endl;
        exit(1);
    }

    string res = "";

    char cur;
    while (read(sock, &cur, 1) > 0) {
        cout << cur;
        res += cur;
    }
    return res;
}

int main() {

    string text;

    std::cout << "Paste your text into the console ";
    getline(cin, text);

    for(int i = 0; i < text.length(); ++i) {
        if (text[i] == ' ') {
            text[i] = '+';
        }
    }

    loadingBar();

    string summary = getSummary(text);

    std::cout << "text: " << text << endl;
    std::cout << "summary of text: " << summary << endl;

    return 0;
}

