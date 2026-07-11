-- bootstrap lazy.nvim, LazyVim and your plugins
require("config.lazy")

-- 1. General Settings
vim.o.background = "dark"
vim.cmd.colorscheme("wal")
vim.g.root_spec = { "cwd" }
vim.opt.foldlevel = 999
vim.opt.foldlevelstart = 999

-- 2. THE STUBBORN SPELLCHECK KILLER
-- This runs every time you open any file or switch buffers
vim.api.nvim_create_autocmd({ "BufEnter", "BufWinEnter", "FileType" }, {
  pattern = "*",
  callback = function()
    vim.opt_local.spell = false
  end,
})

-- 3. Plugin Registration
return {
  {
    "andweeb/presence.nvim",
    config = function()
      require("presence").setup({
        auto_update = true,
        neovim_image_text = "The One True Editor",
        main_image = "neovim",
        editing_text = "Editing %s",
        file_explorer_text = "Browsing %s",
        git_commit_text = "Committing changes",
        plugin_manager_text = "Managing plugins",
        reading_text = "Reading %s",
        workspace_text = "Working on %s",
        line_number_text = "Line %s out of %s",
      })
    end,
  },
}
