"""
Tests for main module
"""

import unittest
from src.main import main


class TestMain(unittest.TestCase):
    """Test cases for main function"""
    
    def test_main_runs(self):
        """Test that main function runs without errors"""
        result = main()
        self.assertEqual(result, 0)


if __name__ == "__main__":
    unittest.main()
