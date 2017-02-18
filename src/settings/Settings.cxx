#include "Settings.h"

TypeHandle Settings::_type_handle;

Settings::Settings() {
    m_vfs = VirtualFileSystem::get_global_ptr();
    m_file = Filename("/useropt"); //First let's use this to set our dir.
    m_file = Filename(m_file.to_os_long_name()); //Now let's do the real file.
    m_file.set_binary();
    
    //Now to define our default settings.
    m_want_music = 1;
    m_want_sfx = 1;
    m_sfx_volume = 100.0f;
    m_music_volume = 100.0f;
    m_force_sw_midi = 0;
    m_embedded_mode = 0;
    m_log_chat = 0;
    m_current_driver = 0;
    m_resolution = 1;
    m_windowed_mode = 0;
    m_resolution_dimensions[0] = 800;
    m_resolution_dimensions[1] = 600;
}

Settings::~Settings() {

}

void Settings::read_settings() {
    Filename found(m_file);
    if (!m_vfs->exists(found)) {
        libotp_cat.debug() << "Failed to find Settings! Creating file...." << std::endl;
        m_vfs->create_file(m_file);
        write_settings();
        return;
    }
    m_vfs->read_file(found, m_data, true);
    m_data = decompress_string(m_data);
    Datagram dg(m_data);
    DatagramIterator dgi(dg);
    m_data = "";
    m_want_music = dgi.get_bool();
    m_want_sfx = dgi.get_bool();
    m_sfx_volume = dgi.get_stdfloat();
    m_music_volume = dgi.get_stdfloat();
    m_force_sw_midi = dgi.get_bool();
    m_embedded_mode = dgi.get_bool();
    m_log_chat = dgi.get_bool();
    m_current_driver = dgi.get_uint8();
    m_resolution = dgi.get_uint8();
    m_windowed_mode = dgi.get_uint8();
    m_resolution_dimensions[0] = dgi.get_uint16();
    m_resolution_dimensions[1] = dgi.get_uint16();
}

void Settings::write_settings() {
    Datagram dg;
    dg.add_bool(m_want_music);
    dg.add_bool(m_want_sfx);
    dg.add_stdfloat(m_sfx_volume);
    dg.add_stdfloat(m_music_volume);
    dg.add_bool(m_force_sw_midi);
    dg.add_bool(m_embedded_mode);
    dg.add_bool(m_log_chat);
    dg.add_uint8(m_current_driver);
    dg.add_uint8(m_resolution);
    dg.add_uint8(m_windowed_mode);
    dg.add_uint16(m_resolution_dimensions[0]);
    dg.add_uint16(m_resolution_dimensions[1]);
    DatagramIterator dgi(dg);
    m_data = dgi.get_remaining_bytes();
    m_data = compress_string(m_data, 9);
    if (m_vfs->exists(m_file)) {
        m_vfs->delete_file(m_file);
    }
    m_vfs->write_file(m_file, m_data, 0);
    m_data = "";
}

void Settings::set_music(bool mode) {
    m_want_music = mode;
}

void Settings::set_sfx(bool mode) {
    m_want_sfx = mode;
}

void Settings::set_force_sw_midi(bool mode) {
    m_force_sw_midi = mode;
}

void Settings::set_embedded_mode(bool mode) {
    m_embedded_mode = mode;
}

void Settings::set_chat_log(bool mode) {
    m_log_chat = mode;
}

void Settings::set_sfx_volume(float volume) {
    m_sfx_volume = volume;
}

void Settings::set_music_volume(float volume) {
    m_music_volume = volume;
}

void Settings::set_display_driver(unsigned int driver) {
    m_current_driver = driver;
}

void Settings::set_windowed_mode(unsigned int mode) {
    m_windowed_mode = mode;
}

void Settings::set_resolution(unsigned int resolution) {
    m_resolution = resolution;
}

void Settings::set_resolution_dimensions(unsigned int xsize, unsigned int ysize) {
    m_resolution_dimensions[0] = xsize;
    m_resolution_dimensions[1] = ysize;
}

int Settings::get_resolution() {
    return m_resolution;
}

int Settings::get_windowed_mode() {
    return m_windowed_mode;
}

bool Settings::get_music() {
    return m_want_music;
}

bool Settings::get_sfx() {
    return m_want_sfx;
}

float Settings::get_sfx_volume() {
    return m_sfx_volume;
}

float Settings::get_music_volume() {
    return m_music_volume;
}

bool Settings::get_embedded_mode() {
    return m_embedded_mode;
}

bool Settings::do_saved_settings_exist() {
    return 0;
}
