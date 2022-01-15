#pragma once

#include "context_obj.h"

namespace noble_steed
{
extern const String CORE_PACKAGE_NAME;
extern const String NONE_LOADED_PACKAGE_NAME;

class Resource : public Context_Obj
{
    friend class Resource_Cache;
  public:
    Resource();

    Resource(const Resource & copy);

    virtual ~Resource();

    virtual bool save();

    // Save data to custom file path - no regards to naming, packages, or extensions.
    virtual bool save(const String & custom_path);

    virtual bool load();

    // Load data from custom_path - no regards to naming, packages, or extensions. Custom path must be valid
    // file to succeed
    virtual bool load(const String & custom_path);

    virtual void initialize(const Variant_Map & init_params);

    virtual void terminate();

    const String & get_display_name();

    void set_display_name(const String & disp_name);

    void set_name(const String & name);

    void set_package(const String & package_name);

    /// Gets the package name in which this resource is owned by
    const String & get_package();

    /// Gets the filename without the exension - if the name has subdirs (name = dir1/dir2/basename) this will just return basename 
    String get_basename();

    /// Gets the filename which is the childmost item in the path with the extension
    String get_filename();

    /// Get's the resource full name which may or may not include dirs (ie dir1/dir2/basename) or just (basename) are both valid
    const String & get_name();

    /// Gets the path, relative to the package directory, without the filename - ie if the name is dir1/dir2/basename this will return dir1/dir2/
    String get_subdir_dir();

    /// Gets the path, relative to the package directory, including the filename
    String get_subdir_path();

    /// Gets the full relative path in relation to cwd - this includes the package, the name (which may or may not have dirs), and the extension
    String get_relative_path();

    /// Gets the path, relative to cwd, of the childmost directory for the resource - ie if package is named dir1/dir2, and name is dir3/dir4/basename, this would return dir1/dir2/dir3/dir4/
    String get_relative_containing_dir_path();

    /// Gets the path, relative to cwd, of the resource excluding the extension
    String get_relative_basename();

    u32 get_id();

    Signal<u32, u32, bool *> change_id;

    Resource & operator=(Resource rhs);

  protected:
    virtual void swap(Resource & rhs);

    String package_;
    String name_;
    String display_name_;
    u32 id_;
};
} // namespace noble_steed