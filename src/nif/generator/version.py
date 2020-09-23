class Version:
    """
    A Fallout NV game engine version.

    
    Attributes:

        Version (int) -- Gamebryo engine version.
        User_Version (int) -- Gamebryo engine user version.
        BS_Header_BS_Version (int) -- Gamebryo engine Bethesda version.

        id (str) -- unique string representation of a version        
        

    Class attributes:

        version_map (dict) -- Maps version id to Gamebryo engine version.
    """

    # Class attribute: dict of int
    version_map = dict()

    def __init__(self, v_id, num, user_num, bs_num):
        self.id = v_id
        self.num = self.get_value(num)
        self.user_num = user_num
        self.bs_num = bs_num

        self.Version = self.num
        self.User_Version = int(self.user_num.split(' ')[0])
        self.BS_Header_BS_Version = int(self.bs_num.split(' ')[0])

    def __str__(self):
        return self.id + '0x{:08X}'.format(self.num)

    @classmethod
    def add_version_to_map(cls, ver_id, num):
        """
            Adds a version number to the version map.
        """
        cls.version_map[ver_id] = cls.get_value(num)

    @staticmethod
    def get_value(string_value):
        """
        Converts a version string into an integer value.
        Each number from the version string is turned into a byte.
        
        Example: '20.0.0.7' is turned into 0x14000007

        Parameters:
            string_value (str) -- version string, e.g. '20.0.0.7'

        Returns:
            int_value (int) -- integer value of the version

        """
        int_value = 0
        parts = [int(value) for value in string_value.split(sep='.')]
        for part, offset in zip(parts, [24, 16, 8, 0]):
            int_value += part << offset
        return int_value

    def is_before(self, ver_id):
        """
            Checks whether the given version is older than this object.
            Ignores user and Bethesda version numbers.

            Parameters:
                ver_id (str) -- version id to compare

            Returns:
                bool
        """

        return self.num <= self.version_map[ver_id]

    def is_after(self, ver_id):
        """
            Checks whether the given version is newer than this object.
            Ignores user and Bethesda version numbers.
            
            Parameters:
                ver_id (str) -- version id to compare
        
            Returns:
                bool
        """
        return self.num >= self.version_map[ver_id]


def create_versions(document, game_name):
    """
        Creates a list of Version objects containing all versions 
        of the given game available in the .xml file.
        
        Parameters:
            document (xml.Document) -- The current document.
            game_name (str) -- Name of the game whose versions to extract.

        Returns:
            versions (list) -- List of extracted versions.
    """

    versions = list()
    for element in document.getElementsByTagName('version'):
        description = element.firstChild.nodeValue.strip()
        Version.add_version_to_map(element.getAttribute('id'), element.getAttribute('num'))
        if game_name in description:
            versions.append(Version(element.getAttribute('id'), element.getAttribute('num'), element.getAttribute('user'), element.getAttribute('bsver')))
    return versions