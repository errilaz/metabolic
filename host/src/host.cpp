#include <iostream>
#include <string>
#include "include/portable-file-dialogs.h"
#include "include/rapidjson/document.h"
#include "include/rapidjson/writer.h"
#include "include/rapidjson/stringbuffer.h"

using namespace std;
namespace json = rapidjson;

struct message {
  string content;
  uint32_t length;
};

json::Document get_message() {
  char raw_length[4];
  fread(raw_length, 4, sizeof(char), stdin);
  uint32_t message_length = *reinterpret_cast<uint32_t*>(raw_length);
  if(!message_length) {
    exit(EXIT_SUCCESS);
  }

  char message[message_length];
  fread(message, message_length, sizeof(char), stdin);
  string m(message, message + sizeof message / sizeof message[0]);
  json::Document doc;
  doc.Parse(m.c_str());
  return doc;
}

message encode_message(json::Document& content) {
  json::StringBuffer buffer;
  json::Writer<json::StringBuffer> writer(buffer);
  content.Accept(writer);
  string encoded = buffer.GetString();
  message message;
  message.content = encoded;
  message.length = (uint32_t) encoded.length();    
  return message;
}

void send_encoded(message encoded_message) {
  char* raw_length = reinterpret_cast<char*>(&encoded_message.length);
  fwrite (raw_length , 4, sizeof(char), stdout);
  fwrite (encoded_message.content.c_str(), encoded_message.length, sizeof(char), stdout);
  fflush(stdout);
}

int main() {
  while (true) {
    const auto message = get_message();
    const string method = message["method"].GetString();
    const auto parameters = message["parameters"].GetArray();

    cerr << method << endl;
    
    if (method == "openFile") {
      auto initialDirectory = parameters[0].GetString();
      // is nullable ^
      cerr << initialDirectory << endl;

      auto open = pfd::open_file("Open");
      auto path = open.result()[0];

      json::Document reply;
      reply.SetObject();
      reply.AddMember("method", "openedFile", reply.GetAllocator());
      json::Value p_path;
      p_path.SetString(path.c_str(), reply.GetAllocator());
      reply.AddMember("parametersTemp", p_path, reply.GetAllocator());
      send_encoded(encode_message(reply));
    }
    else if (method == "saveFileAs") {
      auto initialDirectory = parameters[0].GetString();
    }
    else if (method == "writeFile") {
      auto path = parameters[0].GetString();
      auto text = parameters[1].GetString();
    }
  }
}
