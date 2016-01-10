#define _SCL_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNIGHS

#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WTable>
#include <boost/system/error_code.hpp>
#include <Wt/Http/Client>
#include <Wt/Json/Object>
#include <string>
#include <boost/algorithm/string.hpp>

class HelloApplication : public Wt::WApplication
{
public:
	HelloApplication(const Wt::WEnvironment& env);

private:
	Wt::WLineEdit *nameEdit_;
	Wt::WLineEdit *m_EditReg;
	Wt::WLineEdit *m_EditMake;
	Wt::WLineEdit *m_EditOwner;
	Wt::WLineEdit *m_EditModel;
	Wt::WText *m_WTextRegistration;
	Wt::WText *m_WTextMake;
	Wt::WText *m_WTextModel;
	Wt::WText *m_WTextOwner;
	Wt::WText *greeting_;
	Wt::WTable * tableVehicle;
	void handleHttpResponse(boost::system::error_code err, const Wt::Http::Message& response);
	void greet();

	void searchOwner();
	void searchModel();
	void searchMake();
	void searchVehicle();
	void searchReg();
	void HelloApplication::reloadTable();
};

HelloApplication::HelloApplication(const Wt::WEnvironment& env)
	: Wt::WApplication(env)
{
	setTitle("National stolen vehicles");
	WApplication::enableUpdates();

	root()->addWidget(new Wt::WText("Vehicle_Registration "));
	m_EditReg = new Wt::WLineEdit(root());
	Wt::WPushButton *buttonRegSearch = new Wt::WPushButton("Search", root());
	root()->addWidget(new Wt::WBreak());
	buttonRegSearch->clicked().connect(this, &HelloApplication::searchReg);

	root()->addWidget(new Wt::WText("Make : "));
	m_EditMake = new Wt::WLineEdit(root());
	Wt::WPushButton *buttonMakeSearch = new Wt::WPushButton("Search", root());
	root()->addWidget(new Wt::WBreak());
	buttonMakeSearch->clicked().connect(this, &HelloApplication::searchMake);

	root()->addWidget(new Wt::WText("Model "));
	m_EditModel = new Wt::WLineEdit(root());
	Wt::WPushButton *buttonModelSearch = new Wt::WPushButton("Search", root());
	root()->addWidget(new Wt::WBreak());
	buttonModelSearch->clicked().connect(this, &HelloApplication::searchModel);

	root()->addWidget(new Wt::WText("Owner "));
	m_EditOwner = new Wt::WLineEdit(root());
	Wt::WPushButton *buttonOwnerSearch = new Wt::WPushButton("Search", root());
	root()->addWidget(new Wt::WBreak());
	buttonOwnerSearch->clicked().connect(this, &HelloApplication::searchOwner);

	greeting_ = new Wt::WText(root());
	tableVehicle = new Wt::WTable(root());

	reloadTable();

}

void HelloApplication::searchReg()
{
	greeting_->setText("Searching for Registration:  " + m_EditReg->text());

	Wt::Http::Client *client = new Wt::Http::Client(Wt::WApplication::instance());
	client->done().connect(boost::bind(&HelloApplication::handleHttpResponse, this, _1, _2));
	client->get("http://localhost:5500/?Vehicle_Registration='" + m_EditReg->text().toUTF8() + "'");
	reloadTable();
}
void HelloApplication::searchOwner()
{
	greeting_->setText("Searching for Owner: " + m_EditOwner->text());

	Wt::Http::Client *client = new Wt::Http::Client(Wt::WApplication::instance());
	client->done().connect(boost::bind(&HelloApplication::handleHttpResponse, this, _1, _2));
	client->get("http://localhost:5500/?Vehicle_Owner='" + m_EditOwner->text().toUTF8() + "'");
	reloadTable();

}
void HelloApplication::searchModel()
{
	greeting_->setText("Searching For Model : " + m_EditModel->text());
	Wt::Http::Client *client = new Wt::Http::Client(Wt::WApplication::instance());
	client->done().connect(boost::bind(&HelloApplication::handleHttpResponse, this, _1, _2));
	client->get("http://localhost:5500/?Vehicle_Model='" + m_EditModel->text().toUTF8() + "'");
	reloadTable();
}
void HelloApplication::searchMake()
{
	greeting_->setText("Searching for Make:  " + m_EditMake->text());
	Wt::Http::Client *client = new Wt::Http::Client(Wt::WApplication::instance());
	client->done().connect(boost::bind(&HelloApplication::handleHttpResponse, this, _1, _2));
	client->get("http://localhost:5500/?Vehicle_Make=" + m_EditMake->text().toUTF8() + "");
	reloadTable();
}

void HelloApplication::reloadTable()
{
	int width = 20;
	tableVehicle->clear();
	Wt::WText *vehicleRegistrationText = new Wt::WText("Registration ", tableVehicle->elementAt(0, 0));
	tableVehicle->elementAt(0, 0)->resize(Wt::WLength(width, Wt::WLength::FontEx), Wt::WLength());

	Wt::WText *vehicleRegistrationText2 = new Wt::WText("Make ", tableVehicle->elementAt(0, 1));
	tableVehicle->elementAt(0, 1)->resize(Wt::WLength(width, Wt::WLength::FontEx), Wt::WLength());

	Wt::WText *vehicleRegistrationText3 = new Wt::WText("Model ", tableVehicle->elementAt(0, 2));
	tableVehicle->elementAt(0, 2)->resize(Wt::WLength(width, Wt::WLength::FontEx), Wt::WLength());

	Wt::WText *vehicleRegistrationText4 = new Wt::WText("Owner ", tableVehicle->elementAt(0, 3));
	tableVehicle->elementAt(0, 3)->resize(Wt::WLength(width, Wt::WLength::FontEx), Wt::WLength());
}

void HelloApplication::handleHttpResponse(boost::system::error_code err, const Wt::Http::Message& response) {
	int i = 10;

	std::string responseText = response.body();

	int width = 20;
	std::vector<std::string> list;
	split(list, responseText, boost::is_any_of(L"#"));

	int row = 1, col = 0;
	for (int i = 0; i < list.size(); i++)
	{
		if (i % 4 == 0 && i != 0)	row++;
		
		std::wcout << "Row: " << row << " Colm: " << col << std::endl;
		new Wt::WText(list[i], tableVehicle->elementAt(row, col));
		tableVehicle->elementAt(row, col)->resize(Wt::WLength(width, Wt::WLength::FontEx), Wt::WLength());
		col++;
		if (col == 4)	col = 0;		
	}

	WApplication::triggerUpdate();
}
Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
	return new HelloApplication(env);
}

int main(int argc, char **argv)
{
	return Wt::WRun(argc, argv, &createApplication);
}