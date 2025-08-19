#pragma once

class EngineFilePath
{
public:
	EngineFilePath();
	~EngineFilePath();

private:
	std::filesystem::path FilePath;
};