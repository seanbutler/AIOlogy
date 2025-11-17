"""
Tests for main module
"""

import unittest
from src.main import main


class TestMain(unittest.TestCase):
    """Test cases for main function"""
    
    def test_main_runs(self):
        """Test that main function runs without errors"""
        try:
            main()
            self.assertTrue(True)
        except Exception as e:
            self.fail(f"main() raised {type(e).__name__} unexpectedly!")


if __name__ == "__main__":
    unittest.main()
