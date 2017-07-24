Generator command line args

--threads           [#]             Max number of threads used by the generator
                                    Default: 3

--offMeshInput      [file.*]        Path to file containing off mesh connections data.
                                    Format must be: (see offmesh_example.txt)
                                    "map_id tile_x,tile_y (start_x start_y start_z) (end_x end_y end_z) size  //optional comments"
                                    Single mesh connection per line.

--silent            []              Make us script friendly. Do not wait for user input on error or completion.

--bigBaseUnit       [true|false]    Generate tile/map using bigger basic unit.
                                    Use this option only if you have unexpected gaps.

                                    False: use normal metrics (default)

--maxAngle          [#]             Max walkable inclination angle

                                    Float between 45 and 90 degrees (default 60)

--skipLiquid        [true|false]    Extract liquid data for maps

                                    False: include liquid data (default)

--skipContinents    [true|false]    Continents are maps 0 (Eastern Kingdoms),
                                    1 (Kalimdor), 530 (Outlands), 571 (Northrend)

                                    False: build continents (default)

--skipJunkMaps      [true|false]    Junk maps include some unused
                                    Maps, transport maps, and some other

                                    True: skip junk maps (default)

--skipBattlegrounds [true|false]    Does not include PVP arenas

                                    False: skip battlegrounds (default)

--debugOutput       [true|false]    Create debugging files for use with RecastDemo.
                                    If you are only creating mmaps for use with MaNGOS, you don't want debugging files.

                                    False: don't create debugging files (default)

--tile              [#,#]           Build the specified tile
                                    Seperate number with a comma ','
                                    Must specify a map number (see below)
                                    If this option is not used, all tiles are built

                    [#]             Build only the map specified by #
                                    This command will build the map regardless of --skip* option settings
                                    If you do not specify a map number, builds all maps that pass the filters specified by --skip* options


Examples:
movement_extractor
builds maps using the default settings (see above for defaults)

movement_extractor --skipContinents true
builds the default maps, except continents

movement_extractor 0
builds all tiles of map 0

movement_extractor 0 --tile 34,46
builds only tile 34,46 of map 0 (this is the southern face of blackrock mountain)
