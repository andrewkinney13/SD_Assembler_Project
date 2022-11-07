//
//      Implementation of the Errors class.
//
#include "stdafx.h"
#include "Errors.h"

vector<string> Errors::m_ErrorMsgs;

// Initializes error reports.
void Errors::InitErrorReporting()
{

}

// Records an error message.
void Errors::RecordError(string a_emsg)
{
	m_ErrorMsgs.push_back(a_emsg);
}

// Displays the collected error message.
void Errors::DisplayErrors()
{

}