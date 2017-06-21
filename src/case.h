<<<<<<< HEAD
#ifndef __case_h
#define __case_h

#include <vector>

#include "object.h"
#include "variables.h"
#include "equations.h"
#include "simulation.h"
#include "excelexch.h"
#include "codegenerator.h"
#include "parametric.h"
#include "stochastic.h"
#include "graph.h"

// case events allow the user interface to be updated
// when something internal in the case changes that needs to be reflected
// to the user, i.e. variables are recalculated...

class Case;
class ConfigInfo;

class CaseCallbackContext
{
	Case *m_case;
	wxString m_name;
public:
	CaseCallbackContext( Case *cc, const wxString &name );

	void SetCase( Case *cc, const wxString &name );

	VarTable &GetValues();
	Case &GetCase();
	wxString GetName();

	bool Invoke( lk::node_t *root, lk::env_t *parent );

protected:
	virtual void SetupLibraries( lk::env_t *env );
};

class CaseEvent
{
private:
	int m_type;
	wxArrayString m_vars;
	wxString m_str, m_str2;
public:
	enum { VARS_CHANGED, CONFIG_CHANGED, VALUE_USER_INPUT, SAVE_NOTIFICATION };

	CaseEvent( int type ) : m_type(type) { }
	CaseEvent( int type, const wxString &str ) : m_type(type), m_str(str) { }
	CaseEvent( int type, const wxString &str1, const wxString &str2 ) : m_type(type), m_str(str1), m_str2(str2) { }
	CaseEvent( int type, const wxArrayString &vars ) : m_type(type), m_vars(vars) { }

	int GetType() { return m_type; }
	wxString GetString() { return m_str; }
	wxString GetString2() { return m_str2; }
	wxArrayString &GetVars() { return m_vars; }
};

class CaseEventListener
{
public:
	virtual void OnCaseEvent( Case *, CaseEvent & ) = 0;
};

class Case : public Object
{
public:
	Case();
	virtual ~Case();

	virtual Object *Duplicate();
	virtual bool Copy( Object *obj );
	virtual wxString GetTypeName();
	virtual void Write( wxOutputStream & );
	virtual bool Read( wxInputStream & );

	struct LoadStatus
	{
		LoadStatus() { nread = 0; }
		wxArrayString wrong_type;
		wxArrayString not_found;
		size_t nread;
		wxString error;
	};

	bool LoadValuesFromExternalSource( wxInputStream &in, 
		LoadStatus *di = 0, VarTable *invalids = 0 );

	bool LoadDefaults( wxString *error_msg = 0 );
	bool SaveDefaults( bool quiet = false );

	bool SetConfiguration( const wxString &tech, const wxString &fin, bool silent=false, wxString *message = 0 );
	void GetConfiguration( wxString *tech, wxString *fin );	
	ConfigInfo *GetConfiguration() { return m_config; }
	VarTable &Values() { return m_vals; }
	VarTable &OldValues() { return m_oldVals; }
	VarInfoLookup &Variables();
	EqnFastLookup &Equations();
	wxString GetTechnology() const;
	wxString GetFinancing() const;
	lk::env_t &CallbackEnvironment();
	lk::node_t *QueryCallback( const wxString &method, const wxString &object );
	
	// call this method when a variable is programmatically changed,
	// i.e. through a script callback, SamUL script, or other
	// indirect way of changing a variable.  causes any affected
	// variables to be recalculated, and updates any views
	void VariableChanged( const wxString &name );
	void VariablesChanged( const wxArrayString &list );

	// recalculate any variables that are impacted by a changed value of 'trigger'
	// any views are updated with the variables that are consequently updated with 
	// new values as a result of the calculations
	// Note: if the 'trigger' variable is a library item (has the VF_LIBRARY flag)
	// this will also apply all the library values and cause any affected variables
	// to be subsequently updated
	int Recalculate( const wxString &trigger ); 
	int Recalculate( const wxArrayString &triggers ); 

	// recalculate all equations in this case
	// CaseEvent is issued for all updated variables
	// returns negative on error, or positive number indicating
	// success of number of variables update.  shows an error
	// message box unless 'quietly' = true
	int RecalculateAll( bool quietly = false );
	
	StringHash &Properties() { return m_properties; }
	wxString GetProperty( const wxString &id );
	void SetProperty( const wxString &id, const wxString &value );
	StringHash &Notes() { return m_notes; }
	wxString RetrieveNote( const wxString &id );
	void SaveNote( const wxString &id, const wxString &text );

	void AddListener( CaseEventListener *cel );
	void RemoveListener( CaseEventListener *cel );
	void ClearListeners();
	void SendEvent( CaseEvent e );

	Simulation &BaseCase();

	ExcelExchange &ExcelExch() { return m_excelExch; }
	ParametricData &Parametric() { return m_parametric; }
	StochasticData &Stochastic() { return m_stochastic; }
	void SetGraphs( std::vector<Graph> &gl ) { m_graphs = gl; }
	void GetGraphs( std::vector<Graph> &gl ) { gl = m_graphs; }
	StringHash &Perspective() { return m_perspective; }

private:
	std::vector<CaseEventListener*> m_listeners;

	ConfigInfo *m_config;
	
	// current variable values
	VarTable m_vals;
	// variables that were read in from an old project file made
	// with a previous version of SAM that either don't exist in
	// in the current version configuration, or are of the wrong data
	// type
	VarTable m_oldVals;
	
	lk::env_t m_cbEnv;
	Simulation m_baseCase;
	StringHash m_properties;
	StringHash m_notes;
	ExcelExchange m_excelExch;
	ParametricData m_parametric;
	StochasticData m_stochastic;
	std::vector<Graph> m_graphs;
	StringHash m_perspective;
};

class CaseEvaluator : public EqnEvaluator
{
	Case *m_case;
	VarTable *m_vt;
public:	
	CaseEvaluator( Case *cc, VarTable &vars, EqnFastLookup &efl );
	virtual void SetupEnvironment( lk::env_t &env );	
	virtual int CalculateAll();
	virtual int Changed( const wxArrayString &vars );
	virtual int Changed( const wxString &trigger );

	bool UpdateLibrary( const wxString &trigger, wxArrayString &changed );
};



#endif
=======
#ifndef __case_h
#define __case_h

#include <vector>

#include "object.h"
#include "variables.h"
#include "equations.h"
#include "simulation.h"
#include "excelexch.h"
#include "codegenerator.h"
#include "parametric.h"
#include "stochastic.h"
#include "graph.h"

// case events allow the user interface to be updated
// when something internal in the case changes that needs to be reflected
// to the user, i.e. variables are recalculated...

class Case;
class ConfigInfo;

class CaseCallbackContext
{
	Case *m_case;
	wxString m_name;
public:
	CaseCallbackContext( Case *cc, const wxString &name );

	void SetCase( Case *cc, const wxString &name );

	VarTable &GetValues();
	Case &GetCase();
	wxString GetName();

	bool Invoke( lk::node_t *root, lk::env_t *parent );

protected:
	virtual void SetupLibraries( lk::env_t *env );
};

class CaseEvent
{
private:
	int m_type;
	wxArrayString m_vars;
	wxString m_str, m_str2;
public:
	enum { VARS_CHANGED, CONFIG_CHANGED, VALUE_USER_INPUT, SAVE_NOTIFICATION };

	CaseEvent( int type ) : m_type(type) { }
	CaseEvent( int type, const wxString &str ) : m_type(type), m_str(str) { }
	CaseEvent( int type, const wxString &str1, const wxString &str2 ) : m_type(type), m_str(str1), m_str2(str2) { }
	CaseEvent( int type, const wxArrayString &vars ) : m_type(type), m_vars(vars) { }

	int GetType() { return m_type; }
	wxString GetString() { return m_str; }
	wxString GetString2() { return m_str2; }
	wxArrayString &GetVars() { return m_vars; }
};

class CaseEventListener
{
public:
	virtual void OnCaseEvent( Case *, CaseEvent & ) = 0;
};

class Case : public Object
{
public:
	Case();
	virtual ~Case();

	virtual Object *Duplicate();
	virtual bool Copy( Object *obj );
	virtual wxString GetTypeName();
	virtual void Write( wxOutputStream & );
	virtual bool Read( wxInputStream & );

	struct LoadStatus
	{
		LoadStatus() { nread = 0; }
		wxArrayString wrong_type;
		wxArrayString not_found;
		size_t nread;
		wxString error;
	};

	bool LoadValuesFromExternalSource( wxInputStream &in, 
		LoadStatus *di = 0, VarTable *invalids = 0 );

	bool LoadDefaults( wxString *error_msg = 0 );
	bool SaveDefaults( bool quiet = false );

	bool SetConfiguration( const wxString &tech, const wxString &fin, bool silent=false, wxString *message = 0 );
	void GetConfiguration( wxString *tech, wxString *fin );	
	ConfigInfo *GetConfiguration() { return m_config; }
	VarTable &Values() { return m_vals; }
	VarTable &OldValues() { return m_oldVals; }
	VarInfoLookup &Variables();
	EqnFastLookup &Equations();
	wxString GetTechnology() const;
	wxString GetFinancing() const;
	lk::env_t &CallbackEnvironment();
	lk::node_t *QueryCallback( const wxString &method, const wxString &object );
	
	// call this method when a variable is programmatically changed,
	// i.e. through a script callback, SamUL script, or other
	// indirect way of changing a variable.  causes any affected
	// variables to be recalculated, and updates any views
	void VariableChanged( const wxString &name );
	void VariablesChanged( const wxArrayString &list );

	// recalculate any variables that are impacted by a changed value of 'trigger'
	// any views are updated with the variables that are consequently updated with 
	// new values as a result of the calculations
	// Note: if the 'trigger' variable is a library item (has the VF_LIBRARY flag)
	// this will also apply all the library values and cause any affected variables
	// to be subsequently updated
	int Recalculate( const wxString &trigger ); 
	int Recalculate( const wxArrayString &triggers ); 

	// recalculate all equations in this case
	// CaseEvent is issued for all updated variables
	// returns negative on error, or positive number indicating
	// success of number of variables update.  shows an error
	// message box unless 'quietly' = true
	int RecalculateAll( bool quietly = false );
	
	StringHash &Properties() { return m_properties; }
	wxString GetProperty( const wxString &id );
	void SetProperty( const wxString &id, const wxString &value );
	StringHash &Notes() { return m_notes; }
	wxString RetrieveNote( const wxString &id );
	void SaveNote( const wxString &id, const wxString &text );

	void AddListener( CaseEventListener *cel );
	void RemoveListener( CaseEventListener *cel );
	void ClearListeners();
	void SendEvent( CaseEvent e );

	Simulation &BaseCase();

	ExcelExchange &ExcelExch() { return m_excelExch; }
	ParametricData &Parametric() { return m_parametric; }
	StochasticData &Stochastic() { return m_stochastic; }
	void SetGraphs( std::vector<Graph> &gl ) { m_graphs = gl; }
	void GetGraphs( std::vector<Graph> &gl ) { gl = m_graphs; }
	StringHash &Perspective() { return m_perspective; }

private:
	std::vector<CaseEventListener*> m_listeners;

	ConfigInfo *m_config;
	
	// current variable values
	VarTable m_vals;
	// variables that were read in from an old project file made
	// with a previous version of SAM that either don't exist in
	// in the current version configuration, or are of the wrong data
	// type
	VarTable m_oldVals;
	
	lk::env_t m_cbEnv;
	Simulation m_baseCase;
	StringHash m_properties;
	StringHash m_notes;
	ExcelExchange m_excelExch;
	ParametricData m_parametric;
	StochasticData m_stochastic;
	std::vector<Graph> m_graphs;
	StringHash m_perspective;
};

class CaseEvaluator : public EqnEvaluator
{
	Case *m_case;
	VarTable *m_vt;
public:	
	CaseEvaluator( Case *cc, VarTable &vars, EqnFastLookup &efl );
	virtual void SetupEnvironment( lk::env_t &env );	
	virtual int CalculateAll();
	virtual int Changed( const wxArrayString &vars );
	virtual int Changed( const wxString &trigger );

	bool UpdateLibrary( const wxString &trigger, wxArrayString &changed );
};



#endif
>>>>>>> 2c85b0ce6a18646fb532eb72a604d646517b67ae