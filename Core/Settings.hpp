/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <array>

#include <Core/Config.hpp>
#include <Core/Exception.hpp>
#include <Core/ini/INIFile.hpp>
#include <Core/ConstexprStructures.hpp>
#include <Core/Macros.hpp>

namespace con {

using settingsID_t = int8_t;
enum settings_t : settingsID_t
{
	SETTINGS_DEFAULT_ENGINE,
	SETTINGS_DEFAULT_GAME,

	SETTINGS_ENGINE,
	SETTINGS_GAME
};

#define BEGIN_SETTINGS_DEF \
	static constexpr std::array<constexprRecord_t, MAX_SETTINGS_RECORDS> records = {

#define DEFINE_SETTING( SECTION, NAME, VALUE ) constexprRecord_t( SECTION, NAME, VALUE )

#define END_SETTINGS_DEF }; \
static constexpr constexprString_t Get( const constexprString_t& section, const constexprString_t& name ) \
{ \
	auto result = constexprFindIf( \
		Settings<SETTINGS_DEFAULT_ENGINE>::records.begin(), \
		Settings<SETTINGS_DEFAULT_ENGINE>::records.end(), \
		[section, name]( const constexprRecord_t& record ) \
	{ \
		return ( section == record.section ) && ( name == record.name ); \
	} ); \
	if ( result == Settings<SETTINGS_DEFAULT_ENGINE>::records.end() ) \
		return constexprString_t( "" ); \
	return result->value; \
}

namespace internal
{
	struct SettingsInterface
	{
		virtual bool Load( const std::string& path )
		{
			CON_UNUSED_PARAM( path );
			CON_THROW( "method specially not implemented, don't use it", NotImplemented );
		}
		virtual bool DoesMatchWithDefault()
		{
			CON_THROW( "method specially not implemented, don't use it", NotImplemented );
		}
		virtual bool CreateDefault()
		{
			CON_THROW( "method specially not implemented, don't use it", NotImplemented );
		}
		virtual bool Save()
		{
			CON_THROW( "method specially not implemented, don't use it", NotImplemented );
		}
		virtual std::string* Get( const std::string& section, const std::string& name )
		{
			CON_UNUSED_PARAM( section );
			CON_UNUSED_PARAM( name );
			CON_THROW( "method specially not implemented, don't use it", NotImplemented );
		}

	protected:
		bool load( const std::string path, INIFile& file )
		{
			if ( !file.Open( path ) )
				return false;
			file.Parse();
			return true;
		}
		template <settingsID_t defaultSettingsID>
		bool doesMatchWithDefault( INIFile& file )
		{
			for ( auto& record : Settings<defaultSettingsID>::records )
				if ( !record.SkipWhenSaving() && !file.GetValuePtr( record.section.data, record.name.data ) )
					return false;

			return true;
		}
		template <settingsID_t defaultSettingsID>
		bool createDefault( INIFile& file )
		{
			file.Clear();

			for ( auto& record : Settings<defaultSettingsID>::records )
				if ( !record.SkipWhenSaving() )
					file.AddValue( record.section.data, record.name.data, record.value.data );

			return file.Save();
		}
	};
}
// Dummy
template <settingsID_t T>
struct Settings final :
	internal::SettingsInterface
{};

template <>
struct Settings<SETTINGS_DEFAULT_ENGINE> final
{
	BEGIN_SETTINGS_DEF

		DEFINE_SETTING( "WINDOW", "FPS", "60" ),
		DEFINE_SETTING( "WINDOW", "SIZE_X", "1280" ),
		DEFINE_SETTING( "WINDOW", "SIZE_Y", "720" ),

		DEFINE_SETTING( "AUDIO", "MASTER_VOLUME", "100" ),
		DEFINE_SETTING( "AUDIO", "MUSIC_VOLUME", "100" ),
		DEFINE_SETTING( "AUDIO", "SOUND_VOLUME", "100" ),

		DEFINE_SETTING( "PHYSIC", "UPS", "60" )

		END_SETTINGS_DEF
};

// Redundancy SETTINGS_ENGINE and SETTINGS_GAME...
template<>
struct Settings<SETTINGS_ENGINE> final :
	internal::SettingsInterface
{
	bool Load( const std::string& path ) override
	{
		return this->load( path, this->file );
	}

	bool DoesMatchWithDefault() override
	{
		return this->doesMatchWithDefault<SETTINGS_DEFAULT_ENGINE>( this->file );
	}

	bool CreateDefault() override
	{
		return this->createDefault<SETTINGS_DEFAULT_ENGINE>( this->file );
	}

	bool Save() override
	{
		return this->file.Save();
	}

	std::string* Get( const std::string& section, const std::string& name ) override
	{
		return file.GetValuePtr( section, name );
	}

private:
	INIFile file;
};

template <>
struct Settings<SETTINGS_DEFAULT_GAME> final
{
	BEGIN_SETTINGS_DEF

		DEFINE_SETTING( "DEFAULT", "NAME", "SpaceY" ),
		//...

		END_SETTINGS_DEF
};

template<>
struct Settings<SETTINGS_GAME> final :
	internal::SettingsInterface
{
	bool Load( const std::string& path ) override
	{
		return this->load( path, this->file );
	}

	bool DoesMatchWithDefault() override
	{
		return this->doesMatchWithDefault<SETTINGS_DEFAULT_GAME>( this->file );
	}

	bool CreateDefault() override
	{
		return this->createDefault<SETTINGS_DEFAULT_GAME>( this->file );
	}

	bool Save() override
	{
		return this->file.Save();
	}

	std::string* Get( const std::string& section, const std::string& name ) override
	{
		return file.GetValuePtr( section, name );
	}

private:
	INIFile file;
};
}