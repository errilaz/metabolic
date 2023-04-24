#include <iostream>
#include <string>
#include <fstream>
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

void open_file(const json::Value& defaultPath) {
  auto open = defaultPath.IsNull() 
    ? pfd::open_file("Open").result()
    : pfd::open_file("Open", defaultPath.GetString()).result();
  if (open.empty()) return;

  auto path = open[0].c_str();
  
  json::Document reply;
  reply.SetObject();
  reply.AddMember("method", "openedFile", reply.GetAllocator());
  
  json::Value path_param;
  path_param.SetString(path, reply.GetAllocator());

  json::Value parameters(json::kArrayType);
  parameters.PushBack(path_param, reply.GetAllocator());
  reply.AddMember("parameters", parameters, reply.GetAllocator());

  send_encoded(encode_message(reply));
}

void save_file_as(const json::Value& defaultPath) {
  const string path = defaultPath.IsNull()
    ? pfd::save_file("Save As").result()
    : pfd::save_file("Save As", defaultPath.GetString()).result();
    
  if (path.empty()) return;

  json::Document reply;
  reply.SetObject();
  reply.AddMember("method", "savedFileAs", reply.GetAllocator());

  json::Value path_param;
  path_param.SetString(path.c_str(), reply.GetAllocator());

  json::Value parameters(json::kArrayType);
  parameters.PushBack(path_param, reply.GetAllocator());
  reply.AddMember("parameters", parameters, reply.GetAllocator());

  send_encoded(encode_message(reply));
}

void write_file(const string &path, const string &text) {
  ofstream outfile(path);
  outfile << text;
  outfile.close();

  json::Document reply;
  reply.SetObject();
  reply.AddMember("method", "wroteFile", reply.GetAllocator());

  json::Value path_param;
  path_param.SetString(path.c_str(), reply.GetAllocator());

  json::Value parameters(json::kArrayType);
  parameters.PushBack(path_param, reply.GetAllocator());
  reply.AddMember("parameters", parameters, reply.GetAllocator());

  send_encoded(encode_message(reply));
}

int main() {
  while (true) {
    const auto message = get_message();
    const string method = message["method"].GetString();
    const auto parameters = message["parameters"].GetArray();

    if (method == "openFile") {
      const json::Value& initialDirectory = parameters[0];
      open_file(initialDirectory);
    }
    else if (method == "saveFileAs") {
      const json::Value &initialDirectory = parameters[0];
      save_file_as(initialDirectory);
    }
    else if (method == "writeFile") {
      const auto path = parameters[0].GetString();
      const auto text = parameters[1].GetString();
      write_file(path, text);
    }
  }
}
