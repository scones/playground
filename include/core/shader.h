/*
 * shader.h
 *
 *  Created on: 07.08.2015
 *      Author: scones
 */

#ifndef INCLUDE_CORE_SHADER_H_
#define INCLUDE_CORE_SHADER_H_


#include <cinttypes>
#include <utility>
#include <string>
#include <vector>
#include <unordered_map>
#include <gl/glew.h>


namespace core {


  class shader {

    class activation_object {
      public:


      activation_object(std::uint32_t name);
      activation_object(activation_object&& right);
      ~activation_object();


      protected:


      std::uint32_t m_name;
    };

    public:


    typedef std::vector<std::pair<std::uint32_t, std::string>> t_shader_definition_vector;

    shader(std::uint64_t version, t_shader_definition_vector const& shader_list, std::vector<std::string> uniform_names, std::uint32_t transform_feedback_buffer_mode = GL_INTERLEAVED_ATTRIBS, std::vector<std::string> varyings = std::vector<std::string>())
      :
        m_version(version),
        m_shader_list(shader_list),
        m_uniform_names(uniform_names),
        m_program_name(0),
        m_transform_feedback_buffer_mode(transform_feedback_buffer_mode),
        m_varyings(varyings)
    {
    }
    ~shader() {
      shutdown();
    }

    void init();
    void shutdown();

    std::int32_t get_uniform_location(std::string const& uniform_name);
    std::uint32_t const get_program_name() const { return m_program_name; }

    activation_object activate();


    protected:


    typedef std::unordered_map<std::string, std::int32_t> t_uniform_map;

    std::uint32_t compile_shader(std::uint32_t type, std::string const& shader_name);
    void link_shader();
    static std::uint32_t bits_for_shader_type(std::uint32_t);
    static std::string shader_file_path_name(std::uint32_t type, std::string const& shader_name);
    std::string precompile_shader(std::string const& shader_content);
    std::string precompile_shader_source(std::ostringstream& output_stream, std::string const& source);
    void compilation_control(std::uint32_t shader_name, std::string const& shader_content);
    void linkage_control();

    static std::uint32_t const BUFFER_SIZE = 1024;
    static char buffer[BUFFER_SIZE];

    std::uint64_t m_version;
    t_shader_definition_vector m_shader_list;
    std::vector<std::string> m_uniform_names;
    std::uint32_t m_program_name;
    t_uniform_map m_mapped_uniform_locations;
    std::uint32_t m_transform_feedback_buffer_mode;
    std::vector<std::string> m_varyings;
  };


}


#endif /* INCLUDE_CORE_SHADER_H_ */

