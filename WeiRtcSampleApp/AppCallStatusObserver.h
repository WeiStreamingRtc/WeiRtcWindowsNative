#pragma once

struct AppCallStatusObserver
{
	virtual void OnCallStatus(int status) = 0;
};