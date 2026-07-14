-- Keymaps are automatically loaded on the VeryLazy event
-- Default keymaps that are always set: https://github.com/LazyVim/LazyVim/blob/main/lua/lazyvim/config/keymaps.lua
-- Add any additional keymaps here
vim.keymap.set("n", "gf", function()
  if require("obsidian").util.cursor_on_markdown_link() then
    return "<cmd>ObsidianFollowLink<CR>"
  else
    return "gf"
  end
end, { noremap = false, expr = true })
vim.keymap.set("n", "<C-h>", "<Cmd>TmuxNavigateLeft<CR>", { silent = true })
vim.keymap.set("n", "<C-j>", "<Cmd>TmuxNavigateDown<CR>", { silent = true })
vim.keymap.set("n", "<C-k>", "<Cmd>TmuxNavigateUp<CR>", { silent = true })
vim.keymap.set("n", "<C-l>", "<Cmd>TmuxNavigateRight<CR>", { silent = true })
vim.keymap.set("n", "Zu", "<Cmd>ZenMode<CR>", { silent = true })

vim.keymap.set("n", "Zj", "<Cmd>ObsidianTemplate<CR>", { silent = true })
vim.keymap.set("n", "Zk", "<Cmd>ObsidianPasteImg<CR>", { silent = true })
vim.keymap.set("n", "Zn", "<Cmd>ObsidianNewFromTemplate<CR>", { silent = true })
vim.keymap.set("n", "ZN", "<Cmd>ObsidianNew<CR>", { silent = true })

-- Go to normal mode from terminal with double escape
vim.keymap.set("t", "<Esc><Esc>", "<C-\\><C-n>", { noremap = true })
