#ifndef __TXDPanel__
#define __TXDPanel__

#include "../WxGUI.h"
#include <gtaformats/txd/TXDArchive.h>
#include <fstream>
#include <istream>
#include "../guiconfig.h"
#include "../FileDisplayer.h"

using std::ifstream;
using std::istream;


/** Implementing TXDPanelPrototype */
class TXDPanel : public TXDPanelPrototype
{
protected:
	void onTextureSelectionChanged(wxCommandEvent& evt);
	void onExtract(wxCommandEvent& evt);

/*public:
	TXDPanel(wxWindow* parent, wxWindow* window, wxMenu* menu);
	virtual ~TXDPanel();
	bool displayArchive(istream* stream, bool autoCloseStream = true);
	void displayArchive(TXDArchive* archive);*/

public:
	static bool canDisplay(const wxString& filename);

public:
	TXDPanel(wxWindow* parent);
	virtual ~TXDPanel();
	virtual bool doDisplay(DataSource* source);
	virtual void doClose();
	void displayArchive(TXDArchive* archive);

private:
	void displayTexture(TXDTexture* texture);

private:
	//wxMenu* menu;
	TXDArchive* archive;
	istream* stream;
};

#endif // __TXDPanel__