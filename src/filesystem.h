#pragma once

void CreateEXUDirectory();

void MakeDirectory(const char* directory);

const char* FileRead(const char* fileName);

void FileWrite(const char* fileName, const char* content);