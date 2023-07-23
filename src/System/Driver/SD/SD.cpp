#include "SD.h"

namespace SDLib {

// Used by `getNextPathComponent`
#define MAX_COMPONENT_LEN 12  // What is max length?
#define PATH_COMPONENT_BUFFER_LEN MAX_COMPONENT_LEN + 1

bool getNextPathComponent(const char* path, unsigned int* p_offset, char* buffer) {
    // TODO: Have buffer local to this function, so we know it's the
    //       correct length?

    int bufferOffset = 0;

    int offset = *p_offset;

    // Skip root or other separator
    if (path[offset] == '/') offset++;

    // Copy the next next path segment
    while (bufferOffset < MAX_COMPONENT_LEN && (path[offset] != '/') && (path[offset] != '\0')) {
        buffer[bufferOffset++] = path[offset++];
    }
    buffer[bufferOffset] = '\0';
    if (path[offset] == '/') offset++;
    *p_offset = offset;
    return (path[offset] != '\0');
}

bool walkPath(
    const char* filepath, SdFile& parentDir,
    bool (*callback)(
        SdFile& parentDir, const char* filePathComponent, bool isLastComponent, void* object
    ),
    void* object = NULL
) {
    /*

       When given a file path (and parent directory--normally root),
       this function traverses the directories in the path and at each
       level calls the supplied callback function while also providing
       the supplied object for context if required.

         e.g. given the path '/foo/bar/baz'
              the callback would be called at the equivalent of
        '/foo', '/foo/bar' and '/foo/bar/baz'.

       The implementation swaps between two different directory/file
       handles as it traverses the directories and does not use recursion
       in an attempt to use memory efficiently.

       If a callback wishes to stop the directory traversal it should
       return false--in this case the function will stop the traversal,
       tidy up and return false.

       If a directory path doesn't exist at some point this function will
       also return false and not subsequently call the callback.

       If a directory path specified is complete, valid and the callback
       did not indicate the traversal should be interrupted then this
       function will return true.

    */

    SdFile subfile1;
    SdFile subfile2;

    char buffer[PATH_COMPONENT_BUFFER_LEN];

    unsigned int offset = 0;

    SdFile* p_parent;
    SdFile* p_child;

    SdFile* p_tmp_sdfile;

    p_child = &subfile1;

    p_parent = &parentDir;

    while (true) {
        bool moreComponents = getNextPathComponent(filepath, &offset, buffer);

        bool shouldContinue = callback((*p_parent), buffer, !moreComponents, object);

        if (!shouldContinue) {
            // TODO: Don't repeat this code?
            // If it's one we've created then we
            // don't need the parent handle anymore.
            if (p_parent != &parentDir) {
                (*p_parent).close();
            }
            return false;
        }

        if (!moreComponents) {
            break;
        }

        bool exists = (*p_child).open(*p_parent, buffer, O_RDONLY);

        // If it's one we've created then we
        // don't need the parent handle anymore.
        if (p_parent != &parentDir) {
            (*p_parent).close();
        }

        // Handle case when it doesn't exist and we can't continue...
        if (exists) {
            // We alternate between two file handles as we go down
            // the path.
            if (p_parent == &parentDir) {
                p_parent = &subfile2;
            }

            p_tmp_sdfile = p_parent;
            p_parent = p_child;
            p_child = p_tmp_sdfile;
        } else {
            return false;
        }
    }

    if (p_parent != &parentDir) {
        (*p_parent).close();  // TODO: Return/ handle different?
    }

    return true;
}

/*

   The callbacks used to implement various functionality follow.

   Each callback is supplied with a parent directory handle,
   character string with the name of the current file path component,
   a flag indicating if this component is the last in the path and
   a pointer to an arbitrary object used for context.

*/

bool callback_pathExists(
    SdFile& parentDir, const char* filePathComponent, bool /* isLastComponent */, void* /* object */
) {
    /*

      Callback used to determine if a file/directory exists in parent
      directory.

      Returns true if file path exists.

    */
    SdFile child;

    bool exists = child.open(parentDir, filePathComponent, O_RDONLY);

    if (exists) {
        child.close();
    }

    return exists;
}

bool callback_makeDirPath(
    SdFile& parentDir, const char* filePathComponent, bool isLastComponent, void* object
) {
    /*

      Callback used to create a directory in the parent directory if
      it does not already exist.

      Returns true if a directory was created or it already existed.

    */
    bool result = false;
    SdFile child;

    result = callback_pathExists(parentDir, filePathComponent, isLastComponent, object);
    if (!result) {
        result = child.makeDir(parentDir, filePathComponent);
    }

    return result;
}

bool callback_remove(
    SdFile& parentDir, const char* filePathComponent, bool isLastComponent, void* /* object */
) {
    if (isLastComponent) {
        return SdFile::remove(parentDir, filePathComponent);
    }
    return true;
}

bool callback_rmdir(
    SdFile& parentDir, const char* filePathComponent, bool isLastComponent, void* /* object */
) {
    if (isLastComponent) {
        SdFile f;
        if (!f.open(parentDir, filePathComponent, O_READ)) {
            return false;
        }
        return f.rmDir();
    }
    return true;
}

/* Implementation of class used to create `SDCard` object. */

bool SDClass::begin(uint8_t csPin) {
    if (root.isOpen()) {
        root.close();
    }
    return card.init(SPI_HALF_SPEED, csPin) && volume.init(card) && root.openRoot(volume);
}

bool SDClass::begin(uint32_t clock, uint8_t csPin) {
    if (root.isOpen()) {
        root.close();
    }

    return card.init(SPI_HALF_SPEED, csPin) && card.setSpiClock(clock) && volume.init(card) &&
           root.openRoot(volume);
}

// call this when a card is removed. It will allow you to insert and initialise a new card.
void SDClass::end() { root.close(); }

// this little helper is used to traverse paths
SdFile SDClass::getParentDir(const char* filepath, int* index) {
    // get parent directory
    SdFile d1;
    SdFile d2;

    d1.openRoot(volume);  // start with the mostparent, root!

    // we'll use the pointers to swap between the two objects
    SdFile* parent = &d1;
    SdFile* subdir = &d2;

    const char* origpath = filepath;

    while (strchr(filepath, '/')) {
        // get rid of leading /'s
        if (filepath[0] == '/') {
            filepath++;
            continue;
        }

        if (!strchr(filepath, '/')) {
            // it was in the root directory, so leave now
            break;
        }

        // extract just the name of the next subdirectory
        uint8_t idx = strchr(filepath, '/') - filepath;
        if (idx > 12) {
            idx = 12;  // don't let them specify long names
        }
        char subdirname[13];
        strncpy(subdirname, filepath, idx);
        subdirname[idx] = 0;

        // close the subdir (we reuse them) if open
        subdir->close();
        if (!subdir->open(parent, subdirname, O_READ)) {
            // failed to open one of the subdirectories
            return SdFile();
        }
        // move forward to the next subdirectory
        filepath += idx;

        // we reuse the objects, close it.
        parent->close();

        // swap the pointers
        SdFile* t = parent;
        parent = subdir;
        subdir = t;
    }

    *index = (int)(filepath - origpath);
    // parent is now the parent directory of the file!
    return *parent;
}

IFile* SDClass::open(const char* filepath, uint8_t mode) {
    /*

       Open the supplied file path for reading or writing.

       The file content can be accessed via the `file` property of
       the `SDClass` object--this property is currently
       a standard `SdFile` object from `sdfatlib`.

       Defaults to read only.

       If `write` is true, default action (when `append` is true) is to
       append data to the end of the file.

       If `append` is false then the file will be truncated first.

       If the file does not exist and it is opened for writing the file
       will be created.

       An attempt to open a file for reading that does not exist is an
       error.

    */

    int pathidx;

    IFile* fptr = new File;
    // do the interactive search
    SdFile parentdir = getParentDir(filepath, &pathidx);
    // no more subdirs!

    filepath += pathidx;

    if (!filepath[0]) {
        // it was the directory itself!
        fptr = new File(parentdir, "/");
        return fptr;
    }

    // Open the file itself
    SdFile file;
    // failed to open a subdir!
    if (!parentdir.isOpen() || !file.open(parentdir, filepath, mode)) {
        return fptr;
    }
    // close the parent
    parentdir.close();

    if ((mode & (O_APPEND | O_WRITE)) == (O_APPEND | O_WRITE)) {
        file.seekSet(file.fileSize());
        fptr = new File(file, filepath);
    }
    return fptr;
}

/*
  File SDClass::open(char *filepath, uint8_t mode) {

     Open the supplied file path for reading or writing.

     The file content can be accessed via the `file` property of
     the `SDClass` object--this property is currently
     a standard `SdFile` object from `sdfatlib`.

     Defaults to read only.

     If `write` is true, default action (when `append` is true) is to
     append data to the end of the file.

     If `append` is false then the file will be truncated first.

     If the file does not exist and it is opened for writing the file
     will be created.

     An attempt to open a file for reading that does not exist is an
     error.


  TODO: Allow for read&write? (Possibly not, as it requires seek.)

  fileOpenMode = mode;
  walkPath(filepath, root, callback_openPath, this);

  return File();

  }
*/

// boolean SDClass::close() {
//
//     Closes the file opened by the `open` method.
//
//   file.close();
// }

bool SDClass::exists(const char* filepath) {
    /*

       Returns true if the supplied file path exists.

    */
    return walkPath(filepath, root, callback_pathExists);
}

// boolean SDClass::exists(char *filepath, SdFile& parentDir) {
//
//
//      Returns true if the supplied file path rooted at `parentDir`
//      exists.
//
//
//   return walkPath(filepath, parentDir, callback_pathExists);
// }

uint8_t SDClass::mkdir(const char* filepath) {
    /*

      Makes a single directory or a hierarchy of directories.

      A rough equivalent to `mkdir -p`.

    */
    if (walkPath(filepath, root, callback_makeDirPath)) return 1;
    return 0;
}

uint8_t SDClass::rmdir(const char* filepath) {
    /*

      Remove a single directory or a hierarchy of directories.

      A rough equivalent to `rm -rf`.

    */
    if (walkPath(filepath, root, callback_rmdir)) return 1;
    return 0;
}

bool SDClass::remove(const char* filepath) { return walkPath(filepath, root, callback_remove); }

// allows you to recurse into a directory
File File::openNextFile(uint8_t mode) {
    dir_t p;

    // Serial.print("\t\treading dir...");
    while (_file->readDir(&p) > 0) {
        // done if past last used entry
        if (p.name[0] == DIR_NAME_FREE) {
            return File();
        }

        // skip deleted entry and entries for . and  ..
        if (p.name[0] == DIR_NAME_DELETED || p.name[0] == '.') {
            continue;
        }

        // only list subdirectories and files
        if (!DIR_IS_FILE_OR_SUBDIR(&p)) {
            continue;
        }

        // print file name with possible blank fill
        SdFile f;
        char name[13];
        _file->dirName(p, name);

        if (f.open(_file, name, mode)) {
            return File(f, name);
        } else {
            return File();
        }
    }

    return File();
}

void File::rewindDirectory(void) {
    if (isDirectory()) {
        _file->rewind();
    }
}

SDClass SD;

};  // namespace SDLib