/*
 * Copyright (C) 2013 Marcos Cardinot
 * Copyright (C) 2011 Alexander Wolf
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA  02110-1335, USA.
 */

#ifndef METEORSHOWERS_HPP_
#define METEORSHOWERS_HPP_

#include "StelModule.hpp"
#include "StelObjectModule.hpp"
#include "StelFader.hpp"
#include "StelTextureTypes.hpp"
#include "MeteorShower.hpp"
#include "MeteorStream.hpp"

#include <QDateTime>
#include <QSharedPointer>
#include <QVariantMap>
#include <QFont>

class QNetworkAccessManager;
class QNetworkReply;
class QProgressBar;
class QSettings;
class QTimer;
class MeteorShowerDialog;
class StelButton;
class StelPainter;

typedef QSharedPointer<MeteorShower> MeteorShowerP;

//! This is an example of a plug-in which can be dynamically loaded into stellarium
class MeteorShowers : public StelObjectModule
{
	Q_OBJECT
	Q_PROPERTY(bool msVisible READ getFlagShowMS WRITE setFlagShowMS)
public:
	//! @enum UpdateState
	//! Used for keeping track of the download/update status
	enum UpdateState
	{
		Updating,             //!< Update in progress
		CompleteNoUpdates,    //!< Update completed, there we no updates
		CompleteUpdates,      //!< Update completed, there were updates
		DownloadError,        //!< Error during download phase
		OtherError            //!< Other error
	};

	MeteorShowers();
	virtual ~MeteorShowers();

	///////////////////////////////////////////////////////////////////////////
	// Methods defined in the StelModule class
	virtual void init();
	//! called before the plug-in is un-loaded.
	//! Useful for stopping processes, unloading textures, etc.
	virtual void deinit();
	virtual void update(double deltaTime);
	virtual void draw(StelCore* core);
	virtual void drawMarker(StelCore* core, StelPainter& painter);
	virtual void drawPointer(StelCore* core, StelPainter& painter);
	virtual void drawStream(StelCore* core, StelPainter& painter);
	virtual double getCallOrder(StelModuleActionName actionName) const;

	///////////////////////////////////////////////////////////////////////////
	// Methods defined in StelObjectManager class
	//! Used to get a list of objects which are near to some position.
	//! @param v a vector representing the position in th sky around which to search for nebulae.
	//! @param limitFov the field of view around the position v in which to search for satellites.
	//! @param core the StelCore to use for computations.
	//! @return an list containing the satellites located inside the limitFov circle around position v.
	virtual QList<StelObjectP> searchAround(const Vec3d& v, double limitFov, const StelCore* core) const;

	//! Return the matching satellite object's pointer if exists or NULL.
	//! @param nameI18n The case in-sensistive satellite name
	virtual StelObjectP searchByNameI18n(const QString& nameI18n) const;

	//! Return the matching satellite if exists or NULL.
	//! @param name The case in-sensistive standard program name
	virtual StelObjectP searchByName(const QString& name) const;

	//! Find and return the list of at most maxNbItem objects auto-completing the passed object I18n name.
	//! @param objPrefix the case insensitive first letters of the searched object
	//! @param maxNbItem the maximum number of returned object names
	//! @return a list of matching object name by order of relevance, or an empty list if nothing match
	virtual QStringList listMatchingObjectsI18n(const QString& objPrefix, int maxNbItem=5, bool useStartOfWords=false) const;
	virtual QStringList listMatchingObjects(const QString& objPrefix, int maxNbItem=5, bool useStartOfWords=false) const;
	virtual QStringList listAllObjects(bool inEnglish) const;
	virtual QString getName() const
	{
		return "Meteor Showers";
	}

	//! get a ms object by identifier
	MeteorShowerP getByID(const QString& id);

	//! Implment this to tell the main Stellarium GUI that there is a GUI element to configure this
	//! plugin.
	virtual bool configureGui(bool show=true);

	//! Set up the plugin with default values.  This means clearing out the MeteorShower section in the
	//! main config.ini (if one already exists), and populating it with default values.  It also
	//! creates the default meteors.json file from the resource embedded in the plugin lib/dll file.
	void restoreDefaults(void);

	//! Read (or re-read) settings from the main config file.  This will be called from init and also
	//! when restoring defaults (i.e. from the configuration dialog / restore defaults button).
	void readSettingsFromConfig(void);

	//! Save the settings to the main configuration file.
	void saveSettingsToConfig(void);

	//! get whether or not the plugin will try to update TLE data from the internet
	//! @return true if updates are set to be done, false otherwise
	bool getUpdatesEnabled(void)
	{
		return updatesEnabled;
	}
	//! set whether or not the plugin will try to update TLE data from the internet
	//! @param b if true, updates will be enabled, else they will be disabled
	void setUpdatesEnabled(bool b)
	{
		updatesEnabled=b;
	}

	bool getEnableAtStartup(void) {return enableAtStartup;}
	void setEnableAtStartup(bool b)	{enableAtStartup=b;}

	//! get the date and time the TLE elements were updated
	QDateTime getLastUpdate(void)
	{
		return lastUpdate;
	}

	//! get the update frequency in hours
	int getUpdateFrequencyHours(void)
	{
		return updateFrequencyHours;
	}
	void setUpdateFrequencyHours(int hours)
	{
		updateFrequencyHours = hours;
	}

	//! get the number of seconds till the next update
	int getSecondsToUpdate(void);

	//! Get the current updateState
	UpdateState getUpdateState(void)
	{
		return updateState;
	}

signals:
	//! @param state the new update state.
	void updateStateChanged(MeteorShowers::UpdateState state);

	//! emitted after a JSON update has run.
	void jsonUpdateComplete(void);

public slots:
	//! Download JSON from web recources described in the module section of the
	//! module.ini file and update the local JSON file.
	void updateJSON(void);

	void setFlagShowMS(bool b);
	bool getFlagShowMS(void)
	{
		return flagShowMS;
	}

	//! Display a message. This is used for plugin-specific warnings and such
	void displayMessage(const QString& message, const QString hexColor="#999999");
	void messageTimeout(void);

	//! Define whether the button toggling meteor showers should be visible
	void setFlagShowMSButton(bool b);
	bool getFlagShowMSButton(void)
	{
		return flagShowMSButton;
	}

private:
	// Upgrade config.ini: rename old key settings to new
	void upgradeConfigIni(void);

	//! Check if the sky date was changed
	//! @param core
	//! @return if changed, return true
	bool changedSkyDate(StelCore* core);

	//! Calculate value of ZHR using normal distribution
	//! @param zhr
	//! @param variable
	//! @param start
	//! @param finish
	//! @param peak
	int calculateZHR(int zhr, QString variable, QString start, QString finish, QString peak);

	//! Update the list with information about active meteors
	//! @param core
	void updateActiveInfo(StelCore *core);

	//! Set up the plugin with default values.
	void restoreDefaultConfigIni(void);

	//! replace the json file with the default from the compiled-in resource
	void restoreDefaultJsonFile(void);

	//! read the json file and create the meteor Showers.
	void readJsonFile(void);

	//! Creates a backup of the showers.json file called meteors.json.old
	//! @param deleteOriginal if true, the original file is removed, else not
	//! @return true on OK, false on failure
	bool backupJsonFile(bool deleteOriginal=false);

	//! Get the version from the "version" value in the meteors.json file
	//! @return version string, e.g. "0.1.0"
	const QString getJsonFileVersion(void);

	//! Parse JSON file and load showers to map
	QVariantMap loadShowersMap(QString path=QString());

	//! Set items for list of struct from data map
	void setShowersMap(const QVariantMap& map);

	//! Font used for displaying our text
	QFont font;

	QString showersJsonPath;

	StelTextureSP texPointer;
	QList<MeteorShowerP> mShowers;

	// GUI
	MeteorShowerDialog* configDialog;
	bool flagShowMS;
	bool flagShowMSButton;
	QPixmap* OnIcon;
	QPixmap* OffIcon;
	QPixmap* GlowIcon;
	StelButton* toolbarButton;

	// variables and functions for the updater
	UpdateState updateState;
	QNetworkAccessManager* downloadMgr;
	QString updateUrl;
	QString updateFile;
	class StelProgressController* progressBar;
	QTimer* updateTimer;
	QTimer* messageTimer;
	QList<int> messageIDs;
	bool updatesEnabled;
	QDateTime lastUpdate;
	int updateFrequencyHours;
	bool enableAtStartup;

	QSettings* conf;

	//MS
	std::vector<std::vector<MeteorStream*> > active;		// Matrix containing all active meteors
	int ZHR;
	const static double zhrToWsr = 1.6667f/3600.f;  // factor to convert from zhr to whole earth per second rate
	bool flagShow;

	typedef struct
	{
		QString showerID;		    //! The ID of the meteor shower
		QString year;		        //! Value of year for actual data
		QString start;			    //! First day for activity
		QString finish;			    //! Latest day for activity
		int zhr;		    	    //! ZHR of shower
		QString variable;           //! value of variable for ZHR
		int speed;                  //! Speed of meteors
		QString peak;			    //! Day with maximum for activity
		double radiantAlpha;        //! R.A. for radiant of meteor shower
		double radiantDelta;        //! Dec. for radiant of meteor shower
	} activeData;

	QList<activeData> activeInfo;	//! List of active meteors
	QDateTime skyDate;              //! Current sky date
	QDateTime lastSkyDate;          //! Last sky date

private slots:
	//! check to see if an update is required.  This is called periodically by a timer
	//! if the last update was longer than updateFrequencyHours ago then the update is
	//! done.
	void checkForUpdate(void);
	void updateDownloadComplete(QNetworkReply* reply);

	//void setStelStyle(const QString& section);
};


//#include "fixx11h.h"
#include <QObject>
#include "StelPluginInterface.hpp"

//! This class is used by Qt to manage a plug-in interface
class MeteorShowersStelPluginInterface : public QObject, public StelPluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "stellarium.StelGuiPluginInterface/1.0")
	Q_INTERFACES(StelPluginInterface)
public:
	virtual StelModule* getStelModule() const;
	virtual StelPluginInfo getPluginInfo() const;
};

#endif /*METEORSHOWERS_HPP_*/
