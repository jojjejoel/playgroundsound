#pragma once
class ApplicationTerminator
{
public:
	// Temporary workaround for Wwise DeInit. Proper graceful exit should be fixed in WwiseAPI. 
	void Kill();
};

