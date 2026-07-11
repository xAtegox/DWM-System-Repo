-- Helper to read pywal colors
local function wal_color(n)
  local path = vim.fn.expand("~/.cache/wal/colors.json")
  if vim.fn.filereadable(path) == 0 then
    return "#ffffff"
  end
  local colors = vim.fn.json_decode(table.concat(vim.fn.readfile(path), "\n"))
  return colors.colors["color" .. n] or "#ffffff"
end

-- Override the highlight group used by snacks for the dashboard header
vim.api.nvim_set_hl(0, "SnacksDashboardHeader", { fg = wal_color(4), bold = true })

-- Optional: dynamically update if colorscheme changes
vim.api.nvim_create_autocmd("ColorScheme", {
  callback = function()
    vim.api.nvim_set_hl(0, "SnacksDashboardHeader", { fg = wal_color(4), bold = true })
  end,
})

-- CUSTOM NVIM STARTUP LOOKS
return {
  {
    "folke/snacks.nvim",
    opts = {
      dashboard = {
        preset = {
          header = [[
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣠⣤⣤⣤⣤⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣾⣿⣿⣿⠿⠿⠿⠿⣿⣿⣿⣷⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⣿⠟⠋⠁⠀⠀⠀⠀⠀⠀⠈⠙⠻⣿⣿⣦⡀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⣠⣿⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⣿⣿⣄⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⢠⣿⣿⠏⠀⠀⠀⠀⠀⢀⣠⣾⣷⣄⠀⠀⠀⠀⠀⠀⠹⣿⣿⡄⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⣾⣿⡟⠀⠀⠀⠀⣀⣴⣿⡿⠟⠻⣿⣿⣦⡀⠀⠀⠀⠀⢻⣿⣷⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⣿⣿⡇⠀⠀⠀⠈⠉⠉⢉⣴⣿⣷⣦⡉⠉⠉⠀⠀⠀⠀⢸⣿⣿⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⢿⣿⣧⠀⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣦⣄⠀⠀⠀⠀⣼⣿⡿⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠘⣿⣿⣆⠀⠀⠈⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠁⠀⠀⣰⣿⣿⠃⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠙⣿⣿⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⣿⠋⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠈⠻⣿⣿⣦⣄⡀⠀⠀⠀⠀⠀⠀⢀⣠⣴⣿⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠛⢿⣿⣿⣿⣶⣶⣶⣶⣿⣿⣿⡿⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠙⠛⠛⠛⠛⠋⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
]],
          ---@type snacks.dashboard.Item[]
          keys = {
            { icon = " ", key = "f", desc = "Find Note", action = ":lua Snacks.dashboard.pick('files')" },
            { icon = " ", key = "p", desc = "Find Project", action = ":Telescope projects" },
            {
              icon = " ",
              key = "c",
              desc = "Config",
              action = ":lua Snacks.dashboard.pick('files', {cwd = vim.fn.stdpath('config')})",
            },
            { icon = " ", key = "s", desc = "Restore Session", section = "session" },
            { icon = "󰒲 ", key = "l", desc = "Lazy", action = ":Lazy" },
            { icon = " ", key = "q", desc = "Quit", action = ":qa" },
          },
        },
      },
    },
  },
}
