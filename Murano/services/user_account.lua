--#EVENT user account
-- event
--      .type   'CREATE' | 'DELETE' | 'UPDATE'
--      .id     User ID
--      .email  User email address

if (event.type == 'CREATE') then
  -- handle user create
end

if (event.type == 'DELETE') then
  -- handle user delete
end

if (event.type == 'UPDATE') then
  -- handle user update
end