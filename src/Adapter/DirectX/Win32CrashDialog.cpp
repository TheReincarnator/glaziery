/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


BEGIN_MESSAGE_MAP(Win32CrashDialog, CDialog)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDYES, OnSendClicked)
	ON_BN_CLICKED(IDNO, OnDoNotSendClicked)
END_MESSAGE_MAP()


Win32CrashDialog::Win32CrashDialog(const char * applicationName, const char * applicationVersion,
	const char * exceptionClassName, const char * exceptionMessage,
	const char * exceptionSource, long exceptionLineNumber,
	char ** lastTraceSources, long * lastTraceLineNumbers)
	: CDialog(IDD_CRASHREPORT)
{
	ASSERTION_COBJECT(this);

	this->applicationName = applicationName;
	this->applicationVersion = applicationVersion;
	this->exceptionClassName = exceptionClassName;
	this->exceptionMessage = exceptionMessage;
	this->exceptionSource = exceptionSource;
	this->exceptionLineNumber = exceptionLineNumber;
	this->lastTraceSources = lastTraceSources;
	this->lastTraceLineNumbers = lastTraceLineNumbers;
	restart = false;
}

bool Win32CrashDialog::IsRestart()
{
	ASSERTION_COBJECT(this);
	return restart;
}

void Win32CrashDialog::OnDrawItem(int controlId, LPDRAWITEMSTRUCT drawItemInfo)
{
	ASSERTION_COBJECT(this);

	if (controlId == IDC_CRASHREPORT_ICON)
		balloonImage.Draw(drawItemInfo->hDC);
}

int Win32CrashDialog::OnInitDialog()
{
	ASSERTION_COBJECT(this);

	CDialog::OnInitDialog();

	balloonImage.LoadResource(FindResource(NULL, MAKEINTRESOURCE(IDR_CRASHED), "PNG"), CXIMAGE_FORMAT_PNG);

	CWnd * balloonControl = GetDlgItem(IDC_CRASHREPORT_ICON);
	balloonControl->SetWindowPos(NULL, 0, 0, balloonImage.GetWidth(), balloonImage.GetHeight(),
		SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	CheckDlgButton(IDC_CRASHREPORT_RESTART, true);

	return 1;
}

void Win32CrashDialog::OnDoNotSendClicked()
{
	ASSERTION_COBJECT(this);

	restart = IsDlgButtonChecked(IDC_CRASHREPORT_RESTART) != 0;
	EndDialog(IDOK);
}

void Win32CrashDialog::OnSendClicked()
{
	ASSERTION_COBJECT(this);

	restart = IsDlgButtonChecked(IDC_CRASHREPORT_RESTART) != 0;

	String report;

	String applicationString;
	applicationString.Format("%s %s", applicationName, applicationVersion);
	report += "Application:\n";
	report += applicationString;
	report += "\n\n";

	if (exceptionClassName != NULL)
	{
		report += "Exception:\n";
		report += exceptionClassName;
		if (exceptionMessage != NULL)
		{
			report += ": ";
			report += exceptionMessage;
		}
		if (exceptionSource != NULL)
		{
			report += " @ ";
			report += exceptionSource;
			report += ":";
			report += exceptionLineNumber;
		}
		if (lastTraceSources != NULL)
		{
			report += ", trace:";
			for (int i=0; i<TB_TRACE_LENGTH; i++)
				if (lastTraceSources[i] != NULL)
				{
					report += "\n";
					report += lastTraceSources[i];
					report += ":";
					report += lastTraceLineNumbers[i];
				}
		}
		report += "\n\n";
	}

	CString email;
	GetDlgItemText(IDC_CRASHREPORT_EMAIL, email);
	email = email.Trim();
	if (!email.IsEmpty())
	{
		report += "Email:\n";
		report += (const char *) email;
		report += "\n\n";
	}

	CString note;
	GetDlgItemText(IDC_CRASHREPORT_NOTE, note);
	note = note.Trim();
	if (!note.IsEmpty())
	{
		report += "Note:\n";
		report += (const char *) note;
		report += "\n\n";
	}

	try
	{
		PlatformAdapter * adapter = PlatformAdapter::createInstance();
		if (adapter != NULL)
		{
			report += "System information:\n";
			report += adapter->getSystemInformation();
			report += "\n";

			delete adapter;
		}
	}
	catch (...)
	{
		report += "System information: Unavailable (exception while collecting)\n";
	}

	HttpRequest reportRequest;
	reportRequest.SetHost("software.thomasjacob.de");
	reportRequest.SetMethod(HttpRequest::POST);
	reportRequest.SetPath("/improve/ReportCrashRHAction.do");
	reportRequest.AddParameter("message", report);

	String userAgent;
	userAgent.Format("%s/%s (+http://software.thomasjacob.de)", applicationName, applicationVersion);
	reportRequest.SetUserAgent(userAgent);

	reportRequest.SetTimeout(6000);

	HttpResponse * response = reportRequest.Send();
	if (response != NULL)
		delete response;

	EndDialog(IDOK);
}
