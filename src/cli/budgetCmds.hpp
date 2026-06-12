#pragma once

int cmdInit();
int cmdCreate(int argc, char** argv, int arg_idx, std::string budget);
int cmdAdd(int argc, char** argv, int arg_idx, std::string budget);
int cmdEdit(int argc, char** argv, int arg_idx, std::string budget);
int cmdDelete(int argc, char** argv, int arg_idx, std::string budget);
int cmdShow(int argc, char** argv, int arg_idx, std::string budget);
