std::unique_ptr<ScpiCmdNode> bu6716server::prepareScpiCmdTree() const {
	std::unique_ptr<ScpiCmdNode> cmdTree = std::make_unique<ScpiCmdNode>("root");
	ScpiCmdNode* root = cmdTree.get();
	ScpiCmdNode* navigator;

	root->children.push_back(std::make_unique<ScpiCmdNode>("*idn?", ScpiFunction::getScpiFunction<>(buScpiInterface::IDN_Q)));
	root->children.push_back(std::make_unique<ScpiCmdNode>("*rst", ScpiFunction::getScpiFunction<>(buScpiInterface::RST)));
	root->children.push_back(std::make_unique<ScpiCmdNode>("*tst?", ScpiFunction::getScpiFunction<>(buScpiInterface::TST_Q)));
	root->children.push_back(std::make_unique<ScpiCmdNode>("*opc", ScpiFunction::getScpiFunction<>(buScpiInterface::OPC)));
	root->children.push_back(std::make_unique<ScpiCmdNode>("*opc?", ScpiFunction::getScpiFunction<>(buScpiInterface::OPC_Q)));
	root->children.push_back(std::make_unique<ScpiCmdNode>("*cls", ScpiFunction::getScpiFunction<>(buScpiInterface::CLS)));
	root->children.push_back(std::make_unique<ScpiCmdNode>("*ese", ScpiFunction::getScpiFunction<>(buScpiInterface::ESE)));
	root->children.push_back(std::make_unique<ScpiCmdNode>("*ese?", ScpiFunction::getScpiFunction<>(buScpiInterface::ESE_Q)));
	root->children.push_back(std::make_unique<ScpiCmdNode>("*esr?", ScpiFunction::getScpiFunction<>(buScpiInterface::ESR_Q)));
	root->children.push_back(std::make_unique<ScpiCmdNode>("*sre", ScpiFunction::getScpiFunction<double>(buScpiInterface::SRE)));
	root->children.push_back(std::make_unique<ScpiCmdNode>("*sre?", ScpiFunction::getScpiFunction<>(buScpiInterface::SRE_Q)));
	root->children.push_back(std::make_unique<ScpiCmdNode>("*stb?", ScpiFunction::getScpiFunction<>(buScpiInterface::STB_Q)));
	root->children.push_back(std::make_unique<ScpiCmdNode>("*dsr?", ScpiFunction::getScpiFunction<>(buScpiInterface::DSR_Q)));
	root->children.push_back(std::make_unique<ScpiCmdNode>("*dsr", ScpiFunction::getScpiFunction<double>(buScpiInterface::DSR)));
	root->children.push_back(std::make_unique<ScpiCmdNode>("*wai", ScpiFunction::getScpiFunction<>(buScpiInterface::WAI)));

	root->children.push_back(std::make_unique<ScpiCmdNode>("binternal"));
	root->children.push_back(std::make_unique<ScpiCmdNode>("input", ScpiFunction::getScpiFunction<>(), true, bu6716_NUM_CHAN));
	root->children.push_back(std::make_unique<ScpiCmdNode>("initiate", ScpiFunction::getScpiFunction<>(buScpiInterface::INITIATE)));
	root->children.push_back(std::make_unique<ScpiCmdNode>("sense"));
	root->children.push_back(std::make_unique<ScpiCmdNode>("status"));
	root->children.push_back(std::make_unique<ScpiCmdNode>("system"));

	navigator = root->findCmd(std::vector<std::string>({ "binternal" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("diagnostic"));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("maintenance"));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("hostname?", ScpiFunction::getScpiFunction<>(buScpiInterface::Binternal::HOSTNAME_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("hostname", ScpiFunction::getScpiFunction<std::string>(buScpiInterface::Binternal::HOSTNAME)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("servicename?", ScpiFunction::getScpiFunction<>(buScpiInterface::Binternal::SERVICENAME_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("servicename", ScpiFunction::getScpiFunction<std::string>(buScpiInterface::Binternal::SERVICENAME)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("lanstatus?", ScpiFunction::getScpiFunction<>(buScpiInterface::Binternal::LANSTATUS_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("lanconfig?", ScpiFunction::getScpiFunction<>(buScpiInterface::Binternal::LANCONFIG_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("lanconfig", ScpiFunction::getScpiFunction<unsigned long long, std::string, std::string, std::string, double, double, std::string, std::string>(buScpiInterface::Binternal::LANCONFIG)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("mac?", ScpiFunction::getScpiFunction<>(buScpiInterface::Binternal::MAC_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("ipaddr?", ScpiFunction::getScpiFunction<>(buScpiInterface::Binternal::IPADDR_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("rstr?", ScpiFunction::getScpiFunction<>(buScpiInterface::Binternal::RSTR_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("web"));

	navigator = root->findCmd(std::vector<std::string>({ "binternal",  "maintenance" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("eeprom"));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("fpga", ScpiFunction::getScpiFunction<unsigned long long, unsigned long long>(buScpiInterface::Binternal::Maintenance::FPGA)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("fpga?", ScpiFunction::getScpiFunction<unsigned long long>(buScpiInterface::Binternal::Maintenance::FPGA_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("ic", ScpiFunction::getScpiFunction<unsigned long long, unsigned long long, unsigned long long>(buScpiInterface::Binternal::Maintenance::IC)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("ic?", ScpiFunction::getScpiFunction<unsigned long long, unsigned long long>(buScpiInterface::Binternal::Maintenance::IC_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("icna", ScpiFunction::getScpiFunction<unsigned long long, unsigned long long>(buScpiInterface::Binternal::Maintenance::ICNA)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("icna?", ScpiFunction::getScpiFunction<unsigned long long>(buScpiInterface::Binternal::Maintenance::ICNA_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("ssh", ScpiFunction::getScpiFunction<double>(buScpiInterface::Binternal::Maintenance::SSH)));

	navigator = root->findCmd(std::vector<std::string>({ "binternal",  "maintenance", "eeprom" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("assetno", ScpiFunction::getScpiFunction<std::string>(buScpiInterface::Binternal::Maintenance::Eeprom::ASSETNO)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("dump?", ScpiFunction::getScpiFunction<std::string>(buScpiInterface::Binternal::Maintenance::Eeprom::DUMP_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("pcaprevision", ScpiFunction::getScpiFunction<unsigned long long>(buScpiInterface::Binternal::Maintenance::Eeprom::PCAPREVISION)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("prevision", ScpiFunction::getScpiFunction<unsigned long long>(buScpiInterface::Binternal::Maintenance::Eeprom::PREVISION)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("pcaserial", ScpiFunction::getScpiFunction<unsigned long long>(buScpiInterface::Binternal::Maintenance::Eeprom::PCASERIAL)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("reset", ScpiFunction::getScpiFunction<>(buScpiInterface::Binternal::Maintenance::Eeprom::RESET)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("serial", ScpiFunction::getScpiFunction<unsigned long long>(buScpiInterface::Binternal::Maintenance::Eeprom::SERIAL)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("show?", ScpiFunction::getScpiFunction<>(buScpiInterface::Binternal::Maintenance::Eeprom::SHOW_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("store", ScpiFunction::getScpiFunction<std::string, std::string>(buScpiInterface::Binternal::Maintenance::Eeprom::STORE)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("subtype", ScpiFunction::getScpiFunction<std::string>(buScpiInterface::Binternal::Maintenance::Eeprom::SUBTYPE)));

	navigator = root->findCmd(std::vector<std::string>({ "binternal", "web" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("linkid?", ScpiFunction::getScpiFunction<>(buScpiInterface::Binternal::Web::LINKID_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("password", ScpiFunction::getScpiFunction<std::string, std::string>(buScpiInterface::Binternal::Web::PASSWORD)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("tz", ScpiFunction::getScpiFunction<double>(buScpiInterface::Binternal::Web::TZ)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("tz?", ScpiFunction::getScpiFunction<>(buScpiInterface::Binternal::Web::TZ_Q)));

	navigator = root->findCmd(std::vector<std::string>({ "input" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("autodac", ScpiFunction::getScpiFunction<>(), true, bu6716_NUM_CHAN));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("calibration"));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("coupling", ScpiFunction::getScpiFunction<std::string, std::vector<unsigned long long>>(buScpiInterface::Input::COUPLING)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("coupling?", ScpiFunction::getScpiFunction<std::vector<unsigned long long>>(buScpiInterface::Input::COUPLING_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("excitation"));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("gain", ScpiFunction::getScpiFunction<std::string, std::vector<unsigned long long>>(buScpiInterface::Input::GAIN)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("gain?", ScpiFunction::getScpiFunction<std::vector<unsigned long long>>(buScpiInterface::Input::GAIN_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("mode", ScpiFunction::getScpiFunction<std::string, std::vector<unsigned long long>>(buScpiInterface::Input::MODE)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("mode?", ScpiFunction::getScpiFunction<std::vector<unsigned long long>>(buScpiInterface::Input::MODE_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("shuntresistor", ScpiFunction::getScpiFunction<std::string, std::vector<unsigned long long>>(buScpiInterface::Input::SHUNTRESISTOR)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("shuntresistor?", ScpiFunction::getScpiFunction<std::vector<unsigned long long>>(buScpiInterface::Input::SHUNTRESISTOR_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("teds"));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("smax7318aug", ScpiFunction::getScpiFunction<unsigned long long, std::vector<unsigned long long>>(buScpiInterface::Input::SMAX7318AUG)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("smax7318aug?", ScpiFunction::getScpiFunction<std::vector<unsigned long long>>(buScpiInterface::Input::SMAX7318AUG_Q)));

	navigator = root->findCmd(std::vector<std::string>({ "input", "autodac" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("negative", ScpiFunction::getScpiFunction<double, std::vector<unsigned long long>>(buScpiInterface::Input::AutoDAC::NEGATIVE)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("negative?", ScpiFunction::getScpiFunction<std::vector<unsigned long long>>(buScpiInterface::Input::AutoDAC::NEGATIVE_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("positive", ScpiFunction::getScpiFunction<double, std::vector<unsigned long long>>(buScpiInterface::Input::AutoDAC::POSITIVE)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("positive?", ScpiFunction::getScpiFunction<std::vector<unsigned long long>>(buScpiInterface::Input::AutoDAC::POSITIVE_Q)));

	navigator = root->findCmd(std::vector<std::string>({ "input", "calibration" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("excitation"));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("spath"));

	navigator = root->findCmd(std::vector<std::string>({ "input", "calibration", "excitation" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("negative"));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("positive"));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("reset", ScpiFunction::getScpiFunction<std::string, std::vector<unsigned long long>>(buScpiInterface::Input::Calibration::Excitation::RESET)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("store", ScpiFunction::getScpiFunction<std::vector<unsigned long long>>(buScpiInterface::Input::Calibration::Excitation::STORE)));

	navigator = root->findCmd(std::vector<std::string>({ "input", "calibration", "excitation", "negative" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("gcoefficient", ScpiFunction::getScpiFunction<double, std::vector<unsigned long long>>(buScpiInterface::Input::Calibration::Excitation::Negative::GCOEFFICIENT)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("gcoefficient?", ScpiFunction::getScpiFunction<std::vector<unsigned long long>>(buScpiInterface::Input::Calibration::Excitation::Negative::GCOEFFICIENT_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("offset", ScpiFunction::getScpiFunction<double, std::vector<unsigned long long>>(buScpiInterface::Input::Calibration::Excitation::Negative::OFFSET)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("offset?", ScpiFunction::getScpiFunction<std::vector<unsigned long long>>(buScpiInterface::Input::Calibration::Excitation::Negative::OFFSET_Q)));

	navigator = root->findCmd(std::vector<std::string>({ "input", "calibration", "excitation", "positive" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("gcoefficient", ScpiFunction::getScpiFunction<double, std::vector<unsigned long long>>(buScpiInterface::Input::Calibration::Excitation::Positive::GCOEFFICIENT)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("gcoefficient?", ScpiFunction::getScpiFunction<std::vector<unsigned long long>>(buScpiInterface::Input::Calibration::Excitation::Positive::GCOEFFICIENT_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("offset", ScpiFunction::getScpiFunction<double, std::vector<unsigned long long>>(buScpiInterface::Input::Calibration::Excitation::Positive::OFFSET)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("offset?", ScpiFunction::getScpiFunction<std::vector<unsigned long long>>(buScpiInterface::Input::Calibration::Excitation::Positive::OFFSET_Q)));

	navigator = root->findCmd(std::vector<std::string>({ "input", "calibration", "spath" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("gcoefficient", ScpiFunction::getScpiFunction<std::string, double, std::vector<unsigned long long>>(buScpiInterface::Input::Calibration::SPATH::GCOEFFICIENT)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("gcoefficient?", ScpiFunction::getScpiFunction<std::string, std::vector<unsigned long long>>(buScpiInterface::Input::Calibration::SPATH::GCOEFFICIENT_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("offset", ScpiFunction::getScpiFunction<std::string, double, std::vector<unsigned long long>>(buScpiInterface::Input::Calibration::SPATH::OFFSET)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("offset?", ScpiFunction::getScpiFunction<std::string, std::vector<unsigned long long>>(buScpiInterface::Input::Calibration::SPATH::OFFSET_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("reset", ScpiFunction::getScpiFunction<std::string, std::vector<unsigned long long>>(buScpiInterface::Input::Calibration::SPATH::RESET)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("store", ScpiFunction::getScpiFunction<std::vector<unsigned long long>>(buScpiInterface::Input::Calibration::SPATH::STORE)));

	navigator = root->findCmd(std::vector<std::string>({ "input", "excitation" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("negative", ScpiFunction::getScpiFunction<double, std::vector<unsigned long long>>(buScpiInterface::Input::Excitation::NEGATIVE)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("negative?", ScpiFunction::getScpiFunction<std::vector<unsigned long long>>(buScpiInterface::Input::Excitation::NEGATIVE_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("positive", ScpiFunction::getScpiFunction<double, std::vector<unsigned long long>>(buScpiInterface::Input::Excitation::POSITIVE)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("positive?", ScpiFunction::getScpiFunction<std::vector<unsigned long long>>(buScpiInterface::Input::Excitation::POSITIVE_Q)));

	navigator = root->findCmd(std::vector<std::string>({ "input", "teds" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("ci"));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("cii"));

	navigator = root->findCmd(std::vector<std::string>({ "input", "teds", "ci" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("read"));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("write", ScpiFunction::getScpiFunction<unsigned long long, std::string, std::vector<unsigned long long>>(buScpiInterface::Input::Teds::CI::WRITE)));

	navigator = root->findCmd(std::vector<std::string>({ "input", "teds", "ci", "read" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("eeprom?", ScpiFunction::getScpiFunction<std::vector<unsigned long long>>(buScpiInterface::Input::Teds::CI::Read::EEPROM_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("otprom?", ScpiFunction::getScpiFunction<std::vector<unsigned long long>>(buScpiInterface::Input::Teds::CI::Read::OTPROM_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("rom?", ScpiFunction::getScpiFunction<std::vector<unsigned long long>>(buScpiInterface::Input::Teds::CI::Read::ROM_Q)));
	
	navigator = root->findCmd(std::vector<std::string>({ "input", "teds", "cii" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("read"));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("write", ScpiFunction::getScpiFunction<unsigned long long, std::string, std::vector<unsigned long long>>(buScpiInterface::Input::Teds::CII::WRITE)));

	navigator = root->findCmd(std::vector<std::string>({ "input", "teds", "cii", "read" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("eeprom?", ScpiFunction::getScpiFunction<std::vector<unsigned long long>>(buScpiInterface::Input::Teds::CII::Read::EEPROM_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("otprom?", ScpiFunction::getScpiFunction<std::vector<unsigned long long>>(buScpiInterface::Input::Teds::CII::Read::OTPROM_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("rom?", ScpiFunction::getScpiFunction<std::vector<unsigned long long>>(buScpiInterface::Input::Teds::CII::Read::ROM_Q)));

	navigator = root->findCmd(std::vector<std::string>({ "initiate" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("reset", ScpiFunction::getScpiFunction<>(buScpiInterface::Initiate::RESET)));

	navigator = root->findCmd(std::vector<std::string>({ "sense" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("sensor", ScpiFunction::getScpiFunction<>(), true, bu6716_NUM_CHAN));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("temperature?", ScpiFunction::getScpiFunction<>(buScpiInterface::Sense::TEMPERATURE_Q)));

	navigator = root->findCmd(std::vector<std::string>({ "sense", "sensor" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("iepe"));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("eclimit"));

	navigator = root->findCmd(std::vector<std::string>({ "sense", "sensor", "iepe" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("status?", ScpiFunction::getScpiFunction<std::vector<unsigned long long>>(buScpiInterface::Sense::Sensor::Iepe::STATUS_Q)));

	navigator = root->findCmd(std::vector<std::string>({ "sense", "sensor", "eclimit" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("status?", ScpiFunction::getScpiFunction<std::vector<unsigned long long>>(buScpiInterface::Sense::Sensor::Eclimit::STATUS_Q)));

	navigator = root->findCmd(std::vector<std::string>({ "status" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("cidentify", ScpiFunction::getScpiFunction<std::string, std::vector<unsigned long long>>(buScpiInterface::Status::CIDENTIFY), true, bu6716_NUM_CHAN));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("cidentify?", ScpiFunction::getScpiFunction<std::vector<unsigned long long>>(buScpiInterface::Status::CIDENTIFY_Q), true, bu6716_NUM_CHAN));

	navigator = root->findCmd(std::vector<std::string>({ "system" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("configure"));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("description?", ScpiFunction::getScpiFunction<>(buScpiInterface::System::DESCRIPTION_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("description", ScpiFunction::getScpiFunction<std::string>(buScpiInterface::System::DESCRIPTION)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("assetno?", ScpiFunction::getScpiFunction<>(buScpiInterface::System::ASSETNO_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("error"));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("error?", ScpiFunction::getScpiFunction<>(buScpiInterface::System::ERROR_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("off", ScpiFunction::getScpiFunction<>(buScpiInterface::System::OFF)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("reboot", ScpiFunction::getScpiFunction<>(buScpiInterface::System::REBOOT)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("identify?", ScpiFunction::getScpiFunction<>(buScpiInterface::System::IDENTIFY_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("identify", ScpiFunction::getScpiFunction<double>(buScpiInterface::System::IDENTIFY)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("lanled", ScpiFunction::getScpiFunction<double>(buScpiInterface::System::LANLED)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("tchar?", ScpiFunction::getScpiFunction<>(buScpiInterface::System::TERMCHAR_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("tchar", ScpiFunction::getScpiFunction<unsigned long long>(buScpiInterface::System::TERMCHAR)));

	navigator = root->findCmd(std::vector<std::string>({ "system", "configure" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("default", ScpiFunction::getScpiFunction<>(buScpiInterface::System::Configure::DEFAULT)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("emonitor", ScpiFunction::getScpiFunction<std::string>(buScpiInterface::System::Configure::EMONITOR)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("emonitor?", ScpiFunction::getScpiFunction<>(buScpiInterface::System::Configure::EMONITOR_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("errlines", ScpiFunction::getScpiFunction<>(), true, bu6716_NUM_CHAN));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("reload", ScpiFunction::getScpiFunction<>(buScpiInterface::System::Configure::RELOAD)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("save", ScpiFunction::getScpiFunction<>(buScpiInterface::System::Configure::SAVE)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("vreference"));

	navigator = root->findCmd(std::vector<std::string>({ "system", "configure", "errlines" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("eclimit", ScpiFunction::getScpiFunction<std::string, std::vector<unsigned long long>>(buScpiInterface::System::Configure::Errlines::ECLIMIT)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("eclimit?", ScpiFunction::getScpiFunction<std::vector<unsigned long long>>(buScpiInterface::System::Configure::Errlines::ECLIMIT_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("iepe", ScpiFunction::getScpiFunction<std::string, std::vector<unsigned long long>>(buScpiInterface::System::Configure::Errlines::IEPE)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("iepe?", ScpiFunction::getScpiFunction<std::vector<unsigned long long>>(buScpiInterface::System::Configure::Errlines::IEPE_Q)));

	navigator = root->findCmd(std::vector<std::string>({ "system", "configure", "vreference" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("connect", ScpiFunction::getScpiFunction<double>(buScpiInterface::System::Configure::Vreference::CONNECT)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("connect?", ScpiFunction::getScpiFunction<>(buScpiInterface::System::Configure::Vreference::CONNECT_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("disconnect", ScpiFunction::getScpiFunction<>(buScpiInterface::System::Configure::Vreference::DISCONNECT)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("information?", ScpiFunction::getScpiFunction<>(buScpiInterface::System::Configure::Vreference::INFORMATION_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("mode", ScpiFunction::getScpiFunction<std::string>(buScpiInterface::System::Configure::Vreference::MODE)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("mode?", ScpiFunction::getScpiFunction<>(buScpiInterface::System::Configure::Vreference::MODE_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("temperatures?", ScpiFunction::getScpiFunction<>(buScpiInterface::System::Configure::Vreference::TEMPERATURES_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("voltages"));

	navigator = root->findCmd(std::vector<std::string>({ "system", "configure", "vreference", "voltages" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("active", ScpiFunction::getScpiFunction<double>(buScpiInterface::System::Configure::Vreference::Voltages::ACTIVE)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("active?", ScpiFunction::getScpiFunction<>(buScpiInterface::System::Configure::Vreference::Voltages::ACTIVE_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("supported?", ScpiFunction::getScpiFunction<>(buScpiInterface::System::Configure::Vreference::Voltages::SUPPORTED_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("nominal?", ScpiFunction::getScpiFunction<>(buScpiInterface::System::Configure::Vreference::Voltages::NOMINAL_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("store", ScpiFunction::getScpiFunction<double>(buScpiInterface::System::Configure::Vreference::Voltages::STORE)));

	navigator = root->findCmd(std::vector<std::string>({ "system", "error" }));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("eclimit?", ScpiFunction::getScpiFunction<>(buScpiInterface::System::Error::ECLIMIT_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("iepe?", ScpiFunction::getScpiFunction<>(buScpiInterface::System::Error::IEPE_Q)));
	navigator->children.push_back(std::make_unique<ScpiCmdNode>("next?", ScpiFunction::getScpiFunction<>(buScpiInterface::System::Error::NEXT_Q)));

	log(LOG_DEBUG, cmdTree->toString().c_str());
	return cmdTree;
}
